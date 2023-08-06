#include <bits/stdc++.h>
#include "./OP_init.h"
#include "./utilities.h"
#include "KOB.h"


int main(int argc, char * args []){

/*********************************
 * Create output files
*/
    std::string phase_out_filename = "./data/phase0.csv" ;
    std::ofstream phoutfile (phase_out_filename, std::ios::out) ;
    std::string temp_out_filename = "./data/temp0.csv" ;
    std::ofstream teoutfile (temp_out_filename, std::ios::out) ;

/*********************************
 * Initialize Simulation variables
*/
    int constexpr MESH_X = 512 ;
    int constexpr MESH_Y = MESH_X ;
    int constexpr TOTAL_ITERATIONS = 2000 ;
    int constexpr SAVE_FREQ = 10000 ;

    float constexpr SOLID =  0.0  ;
    float constexpr LIQUID = 1.0 ;
    float constexpr TEMP_SOL = -5.0 ;
    float constexpr TEMP_LIQ = -1.0 ;

    int constexpr NOISE_FREQUENCY = 40 ;
    float constexpr NOISE_AMPLITUDE = 6.1e-2 ;

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
    int constexpr DATA_ALLOC_SIZE_Y = MESH_Y + 2 ;

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
            PHASE_ARRAY0[j][i] = phase_init.init(j,i) ;
            PHASE_ARRAY1[j][i] = phase_init.init(j,i) ;
            TEMP_ARRAY0[j][i]  = temperature_init.init(j,i) ;
            TEMP_ARRAY1[j][i]  = temperature_init.init(j,i) ;
        }
    }

/*********************************
 * output initial phase array
 * Each process is writing to the same file at different offsets
*/  
    utilities::writer writer(0,DATA_ALLOC_SIZE_Y, DATA_ALLOC_SIZE_X) ;
    writer.write<DTYPE>(PHASE_ARRAY0, phoutfile) ;
    writer.write<DTYPE>(TEMP_ARRAY0, teoutfile) ;

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
    long time_start = time(0) ;
    for (iterID = 1 ; iterID <= TOTAL_ITERATIONS ; iterID ++ ){
        add_noise = (iterID % NOISE_FREQUENCY) == 0 ;
        srand(time(0)) ;
        /**
         * Solve PDE
        */
        for (y = 1 ; y < iter_range_y ; y ++){
            for (x = 1; x < iter_range_x ; x ++){

                phase = PHASE_ARRAY0[y][x] ;
                temperature = TEMP_ARRAY0[y][x] ;
                mm = (KOB::ALPHA/3.14152557)*atan( KOB::GAMMA * temperature ) ;

                if ( KOB::check_neighbors<DTYPE>(PHASE_ARRAY0,y,x)){
                    ph_evol_rhs = phase*(1.0-phase)*(phase - 0.5 + mm) ;
                    p2 = phase + (KOB::DT/KOB::TAU)*ph_evol_rhs ;
                    t2 = temperature - KOB::LATENT_HEAT_SLD*(p2-phase) ; 


                }else{
                    phase_lap = KOB::laplacian<DTYPE>(PHASE_ARRAY0,y,x) ;
                    theta = KOB::theta<DTYPE>(PHASE_ARRAY0,y,x) ;
                    eps = KOB::epsilon(theta) ;
                    term1 = KOB::phase_evol_rhs_term1<DTYPE>(PHASE_ARRAY0,y,x) ;

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

        /**
         * Apply Boundary conditions
        */

        for (y = 1 ; y < iter_range_y; y ++){
            for (x = 1; x < iter_range_x; x ++){                
                if ( (x-c)*(x-c) + (y-c)*(y-c) <= r*r ){
                    TEMP_ARRAY0[y][x] = TEMP_SOL ;
                    PHASE_ARRAY0[y][x] = SOLID ;

                }else{
                    TEMP_ARRAY0[y][x] = TEMP_ARRAY1[y][x] ;
                    PHASE_ARRAY0[y][x] = PHASE_ARRAY1[y][x] ;
                }
            }
        }

    }
    long time_end = time(0) ;
    std::cout << 1 << ", " << DATA_ALLOC_SIZE_Y << ", " << time_end-time_start << "\n" ;
    // END of iteration loop
    std::string outfilename = "phase1.csv" ;
    std::ofstream phout1file (outfilename, std::ios::out) ;
    writer.write<DTYPE>(PHASE_ARRAY0,phout1file) ;

    outfilename = "temp1.csv" ;
    std::ofstream tempout1file (outfilename, std::ios::out) ;
    writer.write<DTYPE>(TEMP_ARRAY0,tempout1file) ;



/*********************************
 * End of Program
*/
    return EXIT_SUCCESS ;
}