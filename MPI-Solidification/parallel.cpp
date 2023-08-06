#include <bits/stdc++.h>
#include <mpi.h>

#include "./OP_init.h"
#include "./utilities.h"
#include "KOB.h"

int main(int argc, char **argv) {



    // Parameters for MPI send and recv
    int DESTINATION;
    int SOURCE ;
    int ERROR_INT;
    int size;
    int rank;
    MPI_Status status;
    int TAG1, TAG2, TAG3, TAG4;
    
    // Init system
    ERROR_INT = MPI_Init ( &argc, &argv );
    if ( ERROR_INT != 0 ){
        std::cout << "Error!\n";
        exit ( 1 );
    }

    // get rank and size
    ERROR_INT = MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    ERROR_INT = MPI_Comm_size( MPI_COMM_WORLD, &size );

/*********************************
 * Initialize Simulation variables
*/
    int constexpr MESH_X = DOM_SIZE ;
    int MESH_Y = MESH_X/size ;

    int TOTAL_ITERATIONS = 1000 ;

    float SOLID =  0.0  ;
    float LIQUID = 1.0 ;
    float TEMP_SOL = -5.0 ;
    float TEMP_LIQ = -1.0 ;

    int NOISE_FREQUENCY = 40 ;
    float NOISE_AMPLITUDE = 9.1e-2 ;

    int c = (int) MESH_X / 2 ;
    int r = 10 ;

    OP_init phase_init(c,r,SOLID, LIQUID) ;
    OP_init temperature_init(c,r,TEMP_SOL, TEMP_LIQ) ;


/*********************************
 * Data allocation per processor
 * Partitioning of data happens aloing the Y axis
 * As 2D partitioning is complicated
 * + 2 is added to capture boundary values on both sides
*/
    int constexpr DATA_ALLOC_SIZE_X = MESH_X + 2 ;
    int DATA_ALLOC_SIZE_Y = MESH_Y + 2 ;

/*********************************
 * create simulation arrays
*/
    auto PHASE_ARRAY0 = new float [DATA_ALLOC_SIZE_Y][DATA_ALLOC_SIZE_X] ;
    auto PHASE_ARRAY1 = new float [DATA_ALLOC_SIZE_Y][DATA_ALLOC_SIZE_X] ;

    auto TEMP_ARRAY0 = new float [DATA_ALLOC_SIZE_Y][DATA_ALLOC_SIZE_X] ;
    auto TEMP_ARRAY1 = new float [DATA_ALLOC_SIZE_Y][DATA_ALLOC_SIZE_X] ;

    typedef decltype(PHASE_ARRAY0) DTYPE ;
    // get the type of PHASE_ARRAY0 to be called as DTYPE

/*********************************
 * initialize simulation arrays
*/
    float j_global ;
    for (int j = 0 ; j < DATA_ALLOC_SIZE_Y ; j++){
        for (int i = 0 ; i < DATA_ALLOC_SIZE_X ; i++ ){
            j_global = rank*DATA_ALLOC_SIZE_Y + j ;
            PHASE_ARRAY0[j][i] = phase_init.init(j_global,i) ;
            PHASE_ARRAY1[j][i] = phase_init.init(j_global,i) ;
            TEMP_ARRAY0[j][i]  = temperature_init.init(j_global,i) ;
            TEMP_ARRAY1[j][i]  = temperature_init.init(j_global,i) ;
        }
    }

/*********************************
 * output initial phase array
 * Each process is writing to the same file at different offsets
*/  
    utilities::writer writer(0,DATA_ALLOC_SIZE_Y, DATA_ALLOC_SIZE_X) ;

    std::string phoutfilen = "./data/ph0"+std::to_string(rank)+".csv" ;
    std::ofstream phoutfile0 (phoutfilen, std::ios::out) ;
    std::string teoutfilen = "./data/temp0"+std::to_string(rank)+".csv" ;
    std::ofstream teoutfile0 (teoutfilen, std::ios::out) ;

    writer.write<DTYPE>(PHASE_ARRAY0, phoutfile0) ;
    writer.write<DTYPE>(TEMP_ARRAY0, teoutfile0) ;

/*********************************
 * Iterate 
*/  

    int iter_range_x = DATA_ALLOC_SIZE_X -2 ;
    int iter_range_y = DATA_ALLOC_SIZE_Y -2 ;

/**
 * create the variables instead of creating them everytime
*/
    int iterID = 0 ;
    int x = 0 , y = 0 ; 
    float phase, phase_lap, theta, eps, term1 ;
    float temperature, mm , ph_evol_rhs ;
    float t2, p2 ;

    bool add_noise = false ;
    int TOP_PARTNER    = rank == 0      ? MPI_PROC_NULL : rank - 1 ;
    int BOTTOM_PARTNER = rank == size-1 ? MPI_PROC_NULL : rank + 1 ;

    long time_start = time(0) ;

    for (iterID = 1 ; iterID <= TOTAL_ITERATIONS ; iterID ++ ){
        add_noise = (iterID % NOISE_FREQUENCY) == 0 ;
        srand(time(0)) ;
        
        MPI_Barrier(MPI_COMM_WORLD) ;
        
        for (y = 1 ; y <= iter_range_y ; y ++){
            for (x = 1; x <= iter_range_x ; x ++){

                phase = PHASE_ARRAY0[y][x] ;
                temperature = TEMP_ARRAY0[y][x] ;
                mm = (KOB::ALPHA/3.14152557)*atan( KOB::GAMMA * temperature ) ;

                if ( KOB::check_neighbors<DTYPE>(PHASE_ARRAY0,y,x)){
                    ph_evol_rhs = phase*(1.0-phase)*(phase - 0.5 + mm) ;
                    p2 = phase + (KOB::DT/KOB::TAU)*ph_evol_rhs ;
                    t2 = temperature - KOB::LATENT_HEAT_SLD*(p2-phase) ; 


                }else{
                    phase_lap = KOB::laplacian<DTYPE>(PHASE_ARRAY0,y,x) ;
                    theta     = KOB::theta<DTYPE>(PHASE_ARRAY0,y,x) ;
                    eps       = KOB::epsilon(theta) ;
                    term1     = KOB::phase_evol_rhs_term1<DTYPE>(PHASE_ARRAY0,y,x) ;

                    ph_evol_rhs = term1 + eps*eps*phase_lap + phase*(1.0-phase)*(phase - 0.5 + mm) ;

                    p2 = phase + (KOB::DT/KOB::TAU)*ph_evol_rhs;


                    t2 = temperature + KOB::DT*KOB::THERMAL_DIFFUSIVITY*KOB::laplacian(TEMP_ARRAY0,y,x) ;
                    t2 -= KOB::LATENT_HEAT_SLD*(p2-phase) ;                    
                }

                if(add_noise){
                    p2 += 10*p2*(1.0-p2)*NOISE_AMPLITUDE*(   0.5 - (float)rand()/(float)RAND_MAX );
                }

                PHASE_ARRAY1[y][x] = p2 < 0.0 ? 0.0 : p2 > 1.0 ? 1.0 : p2 ;
                TEMP_ARRAY1[y][x]  = t2 ;            
            }
        }


        // Send and recieve boundary values
        TAG1 = 13*iterID ;
        TAG2 = TAG1 + 2 ;
        TAG3 = TAG1 + 5 ;
        TAG4 = TAG1 + 7 ;
        ERROR_INT = MPI_Send(PHASE_ARRAY0[1]                   ,DATA_ALLOC_SIZE_X,MPI_FLOAT,TOP_PARTNER   ,TAG1,MPI_COMM_WORLD) ;
        ERROR_INT = MPI_Send(PHASE_ARRAY0[DATA_ALLOC_SIZE_Y-2] ,DATA_ALLOC_SIZE_X,MPI_FLOAT,BOTTOM_PARTNER,TAG2,MPI_COMM_WORLD) ;
        
        ERROR_INT = MPI_Send(TEMP_ARRAY0[1]                   ,DATA_ALLOC_SIZE_X,MPI_FLOAT,TOP_PARTNER   ,TAG3,MPI_COMM_WORLD) ;
        ERROR_INT = MPI_Send(TEMP_ARRAY0[DATA_ALLOC_SIZE_Y-2] ,DATA_ALLOC_SIZE_X,MPI_FLOAT,BOTTOM_PARTNER,TAG4,MPI_COMM_WORLD) ;
        
        ERROR_INT = MPI_Recv(PHASE_ARRAY1[0]                   ,DATA_ALLOC_SIZE_X,MPI_FLOAT,TOP_PARTNER   ,TAG2,MPI_COMM_WORLD,&status);
        ERROR_INT = MPI_Recv(PHASE_ARRAY1[DATA_ALLOC_SIZE_Y-1] ,DATA_ALLOC_SIZE_X,MPI_FLOAT,BOTTOM_PARTNER,TAG1,MPI_COMM_WORLD,&status);

        ERROR_INT = MPI_Recv(TEMP_ARRAY1[0]                   ,DATA_ALLOC_SIZE_X,MPI_FLOAT,TOP_PARTNER   ,TAG4,MPI_COMM_WORLD,&status);
        ERROR_INT = MPI_Recv(TEMP_ARRAY1[DATA_ALLOC_SIZE_Y-1] ,DATA_ALLOC_SIZE_X,MPI_FLOAT,BOTTOM_PARTNER,TAG3,MPI_COMM_WORLD,&status);

        for (y = 0 ; y < DATA_ALLOC_SIZE_Y ; y ++){
            for (x = 0; x < DATA_ALLOC_SIZE_X ; x ++){
                PHASE_ARRAY0[y][x] = PHASE_ARRAY1[y][x] ;
                TEMP_ARRAY0[y][x]  = TEMP_ARRAY1[y][x] ;
            }
        }

    }

    MPI_Barrier(MPI_COMM_WORLD) ;
    long time_end = time(0) ;
    if (rank == 0){
        std::cout << size << ", " << MESH_X << ", " << time_end-time_start << "\n" ;
    }

    phoutfilen = "./data/ph1"+std::to_string(rank)+".csv" ;
    std::ofstream phoutfile1 (phoutfilen, std::ios::out) ;
    teoutfilen = "./data/temp1"+std::to_string(rank)+".csv" ;
    std::ofstream teoutfile1 (teoutfilen, std::ios::out) ;

    writer.write<DTYPE>(PHASE_ARRAY0, phoutfile1) ;
    writer.write<DTYPE>(TEMP_ARRAY0, teoutfile1) ;
    
    MPI_Finalize() ;

    return EXIT_SUCCESS ;
}