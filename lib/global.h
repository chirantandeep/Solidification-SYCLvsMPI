#ifndef pfmodelh
#define pfmodelh

#include "../src/compile_time_vars.h"

namespace gvars{
    class mesh
    {   
        public: 
            sycl::int2 alloc_range ; 
            sycl::int2 iter_range ;
            sycl::int2 offset ;
            float deltax ;
            float deltay ;
            float deltat ;
            sycl::range<2> get_alloc_range(){
                auto ar = alloc_range ;
                sycl::range<2> arange(ar[0], ar[1]) ; 
                return arange;
            }

            sycl::range<2> get_iter_range(){
                auto ir = iter_range ;
                sycl::range<2> irange(ir[0], ir[1]) ; 
                return irange;
            }
    };

    struct control
    {
        int iterations ;
        int nsave ;
        int noise_freq ;
        float noise_amp ;
        std::string saveas ;
    };
    
    struct init
    {
        std::string style ;
        float pos_x ;
        float pos_y ;
        float size_x ;
        float size_y ;
    };

    /// @brief 
    struct kob{
        float pi ;
        float eps0 ;
        float alpha ;
        float gamma ;
        float delta ; 
        float J ; 
        float tau ; 
        float theta0 ;
        float diff_coeff ; 
        float latent_heat ; 

        float solid ;
        float liquid ;
        float temp_sol ;
        float temp_liq ;
    };

    /// @brief 
    struct operator_arrays
    {
        float laplacian[CT_VARS::STENCIL_SIZE];
    };

    /// @brief 
    struct barrier_potential_func_arrays
    {
        float coeff[CT_VARS::MAX_BARRIER_POTENTIAL_TERMS] ;
        int phi_pow[CT_VARS::MAX_BARRIER_POTENTIAL_TERMS] ;
        int  mm_pow[CT_VARS::MAX_BARRIER_POTENTIAL_TERMS] ;
    };
}

namespace kern{
    class data{
        public:
        sycl::int2 ar ; 
        sycl::int2 ir ;
        sycl::int2 off ;
        float dy ;
        float dx ;
        float dt ;
        float noise = 0.0;
        gvars::kob kv ;
        gvars::operator_arrays oa ;
        gvars::barrier_potential_func_arrays bpfa ;

        /// @brief 
        /// @param mmm 
        /// @param kkk 
        /// @param oooo 
        /// @param bbbb 
        data(
            gvars::mesh mmm,
            gvars::kob  kkk,
            gvars::operator_arrays oooo,
            gvars::barrier_potential_func_arrays bbbb) :
            ar(mmm.alloc_range),
            ir(mmm.iter_range),
            off(mmm.offset),
            dy(mmm.deltay),
            dx(mmm.deltax),
            dt(mmm.deltat),
            kv(kkk),
            oa(oooo),
            bpfa(bbbb) {}
    };
    
}

#endif