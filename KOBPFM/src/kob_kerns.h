#ifndef kobayashikernsh
#define kobayashikernsh

#include "../lib/global.h"
#include "../lib/kob_op.h"
#include "../lib/pde_op.h"
#include "../lib/pfm_op.h"

namespace kern{

    class kobayashi_aniso
    {
    private :
        data kdat ;
        sycl::accessor<float,2> phi_inp  ;
        sycl::accessor<float,2> phi_out  ;
        sycl::accessor<float,2> temp_inp ;
        sycl::accessor<float,2> temp_out ;

    public:
        kobayashi_aniso(
            data vars,
            sycl::accessor<float,2> one,
            sycl::accessor<float,2> two,
            sycl::accessor<float,2> three,
            sycl::accessor<float,2> four) :
            kdat(vars),
            phi_inp(one),
            phi_out(two),
            temp_inp(three),
            temp_out(four) {}

        void operator()(sycl::id<2> ID) const {
            int y = ID[0] + kdat.off[0] ; 
            int x = ID[1] + kdat.off[1] ;

            float phase ;
            float temperature, mm, barrier_term;
            float t2, p2 ;

            phase = phi_inp[y][x] ;
            temperature = temp_inp[y][x] ;
            mm = (kdat.kv.alpha/kdat.kv.pi)*sycl::atan(kdat.kv.gamma * temperature) ;
            //barrier_term = kobop::barrier_potential(phase,mm) ;
            barrier_term = kobop::barrier_potential<CT_VARS::MAX_BARRIER_POTENTIAL_TERMS>(kdat.bpfa, phase, mm) ;

            if(pfmop::check_neighbors(phi_inp,y,x)){
                p2 = phase + (kdat.dt/kdat.kv.tau)*(barrier_term) ;
                t2 = temperature - kdat.kv.latent_heat*(p2-phase); 
            }else{
                float phi_stencil[CT_VARS::STENCIL_SIZE], temp_stencil[CT_VARS::STENCIL_SIZE] ;
                pdeop::get_stencil<CT_VARS::STENCIL_SIZE>(phi_inp ,y,x, phi_stencil) ;
                pdeop::get_stencil<CT_VARS::STENCIL_SIZE>(temp_inp,y,x, temp_stencil) ;

                float  phase_lap, eps, term1 ;
                phase_lap  = pdeop::operator_dot_product<CT_VARS::STENCIL_SIZE>(kdat.oa.laplacian,phi_stencil) ;
                phase_lap *= 1.0/(kdat.dx*kdat.dy) ;
                eps        = kobop::epsilon(kdat, kobop::theta(phi_stencil) ) ;
                
                float tmp1, tmp2 ;
                tmp1 = 0.5*(phi_stencil[5]-phi_stencil[6])*kobop::epsDepsDtheta(kdat, phi_stencil[2]-phi_stencil[0] , 0.5*(phi_stencil[5]-phi_stencil[6])) ;
                tmp2 = 0.5*(phi_stencil[8]-phi_stencil[7])*kobop::epsDepsDtheta(kdat, phi_stencil[0]-phi_stencil[4] , 0.5*(phi_stencil[8]-phi_stencil[7])) ;
                term1 = (tmp1 - tmp2)/(2.0*kdat.dy*kdat.dx) ;

                tmp1 = 0.5*(phi_stencil[5]-phi_stencil[8])*kobop::epsDepsDtheta(kdat, 0.5*(phi_stencil[5]-phi_stencil[8]) , phi_stencil[1]-phi_stencil[0]) ;
                tmp2 = 0.5*(phi_stencil[6]-phi_stencil[7])*kobop::epsDepsDtheta(kdat, 0.5*(phi_stencil[6]-phi_stencil[7]) , phi_stencil[0]-phi_stencil[3]) ;
                term1 -= (tmp1 - tmp2)/(2.0*kdat.dx*kdat.dy) ;

                p2  = phase + (kdat.dt/kdat.kv.tau)*(term1 + eps*eps*phase_lap + barrier_term) ;
                
                // Temperature evolution
                t2  = pdeop::operator_dot_product<CT_VARS::STENCIL_SIZE>(kdat.oa.laplacian,temp_stencil) ;
                t2 *= kdat.dt*kdat.kv.diff_coeff/(kdat.dx*kdat.dy) ;
                t2 += temperature - kdat.kv.latent_heat*(p2-phase) ;
            }

            if(kdat.noise == 0.0){
                phi_out[y][x] = p2 ;
            }else{
                std::uint64_t offset = 2 ; //x + y ;
                oneapi::dpl::minstd_rand engine ( 2 , offset ) ;
                oneapi::dpl::uniform_real_distribution<float> distr ;
                float rand_float = distr ( engine ) ;
                rand_float = rand_float - 0.5 ;
                p2 += phase*(1.0-phase)*kdat.noise*rand_float ;
                p2 = p2 >= 1.0 ? 1.0 : p2 <= 0.0 ? 0.0 : p2 ; 
                phi_out[y][x] = p2 ;   
            }
            temp_out[y][x] = t2 ; 
        }
    };
}
#endif