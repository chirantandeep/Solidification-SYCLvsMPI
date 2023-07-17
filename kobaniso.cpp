#include <CL/sycl.hpp>
//#include <oneapi/dpl/execution>
//#include <oneapi/dpl/algorithm>
#include <oneapi/dpl/random>
#include <bits/stdc++.h>

#include "./src/compile_time_vars.h"
#include "./lib/io.h"
#include "./lib/initialize.h"
#include "./lib/utils.h"
#include "./lib/enqueue.h"
#include "./lib/dynamic_read.h"

int main(int argc, char * args[]){

    gvars::init ivar    = io::read_init_file(args[1]) ;
    gvars::mesh mvar    = io::read_mesh_vars(args[1]) ;
    gvars::kob kvar     = io::read_kobayashi_vars(args[1]) ;
    gvars::control cvar = io::read_control_vars(args[1]) ;
    auto pfar = io::read_barrier_potential_func(args[1]) ;
    auto opar = io::read_operator_arrays(args[1]) ;

    kern::data kdat(mvar, kvar, opar, pfar) ;

    sycl::queue Q = get::gpu() ;

    std::ifstream progfile(args[2]) ;
    std::string line ;
    std::cout << "runfile from : " << args[2] << "\n" ;

    // Begin Buffer block
    {
        auto phi_now   = initialize::field(mvar, ivar, kvar.solid, kvar.liquid);
        auto temp_now  = initialize::field(mvar, ivar, kvar.temp_sol, kvar.temp_liq);
        auto phi_next  = initialize::field(mvar) ;
        auto temp_next = initialize::field(mvar) ;

        int iterID = 0 ;
        while(std::getline(progfile, line)){
            if( (iterID==0) || dynamic_read::is_run_command(line, &cvar)){
                for(; iterID <= cvar.iterations ; iterID ++){
        
                    if (iterID % (cvar.nsave) == 0.0) io::save(cvar.saveas, mvar, phi_now, "phi" + std::to_string(iterID));
                    kdat.noise = iterID % cvar.noise_freq ? cvar.noise_amp : 0.0 ;

                    enq::solver<kern::kobayashi_aniso>(Q, mvar, kdat, phi_now, phi_next, temp_now, temp_next).wait() ;
                    enq::bc<kern::bc::periodic_x,0>(Q, mvar, kdat, phi_next, temp_next).wait();
                    enq::bc<kern::bc::periodic_y,1>(Q, mvar, kdat, phi_next, temp_next).wait();
                    enq::solver<kern::kobayashi_aniso>(Q, mvar, kdat, phi_next, phi_now, temp_next, temp_now).wait() ;
                    enq::bc<kern::bc::periodic_x,0>(Q, mvar, kdat, phi_now, temp_now).wait();
                    enq::bc<kern::bc::periodic_y,1>(Q, mvar, kdat, phi_now, temp_now).wait();

                }
            }else if(dynamic_read::is_set_command(line, &kdat.kv, &cvar)){;}
        }
    }
    std::cout << "main>> Run successful\n";
    return EXIT_SUCCESS ; 
}