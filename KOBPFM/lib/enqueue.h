#ifndef enqueueh
#define enqueueh

#include "bc_kerns.h"
#include "../src/kob_kerns.h"

namespace enq{
    template<class kern_func_class>
    static inline sycl::event solver(
        sycl::queue Q, gvars::mesh m,
        kern::data kdat,
        sycl::buffer<float,2> one){
            return Q.submit([&](sycl::handler& h){
                sycl::accessor one_acc{one,h} ;
                kern_func_class the_kern_operator(kdat, one_acc) ;
                h.parallel_for(m.get_iter_range(), the_kern_operator );
            });
    }

    template<class kern_func_class>
    static inline sycl::event solver(
        sycl::queue Q, gvars::mesh m,
        kern::data kdat,
        sycl::buffer<float,2> one,
        sycl::buffer<float,2> two){
            return Q.submit([&](sycl::handler& h){
                sycl::accessor one_acc{one,h} ;
                sycl::accessor two_acc{two,h} ;
                kern_func_class the_kern_operator(kdat, one_acc, two_acc) ;
                h.parallel_for(m.get_iter_range(), the_kern_operator );
            });
    }

    template<class kern_func_class>
    static inline sycl::event solver(
        sycl::queue Q, gvars::mesh m,
        kern::data kdat,
        sycl::buffer<float,2> one,
        sycl::buffer<float,2> two,
        sycl::buffer<float,2> three){

            return Q.submit([&](sycl::handler& h){
                sycl::accessor one_acc{one,h} ;
                sycl::accessor two_acc{two,h} ;
                sycl::accessor three_acc{three,h} ;
                kern_func_class the_kern_operator(kdat, one_acc, two_acc, three_acc) ;
                h.parallel_for(m.get_iter_range(), the_kern_operator );
            });
    }

    template<class kern_func_class>
    static inline sycl::event solver(
        sycl::queue Q, gvars::mesh m,
        kern::data kdat,
        sycl::buffer<float,2> one,
        sycl::buffer<float,2> two,
        sycl::buffer<float,2> three,
        sycl::buffer<float,2> four){

            return Q.submit([&](sycl::handler& h){
                sycl::accessor one_acc{one,h} ;
                sycl::accessor two_acc{two,h} ;
                sycl::accessor three_acc{three,h} ;
                sycl::accessor four_acc{four,h} ;
                kern_func_class the_kern_operator(kdat, one_acc, two_acc, three_acc, four_acc) ;
                h.parallel_for(m.get_iter_range(), the_kern_operator );
            });
    }

    template<class kern_func_class>
    static inline sycl::event solver(
        sycl::queue Q, gvars::mesh m,
        kern::data kdat,
        sycl::buffer<float,2> one,
        sycl::buffer<float,2> two,
        sycl::buffer<float,2> three,
        sycl::buffer<float,2> four,
        sycl::buffer<float,2> five){

            return Q.submit([&](sycl::handler& h){
                sycl::accessor one_acc{one,h} ;
                sycl::accessor two_acc{two,h} ;
                sycl::accessor three_acc{three,h} ;
                sycl::accessor four_acc{four,h} ;
                sycl::accessor five_acc{five,h} ;
                kern_func_class the_kern_operator(kdat, one_acc, two_acc, three_acc, four_acc, five_acc) ;
                h.parallel_for(m.get_iter_range(), the_kern_operator );
            });
    }

    template<class kern_func_class>
    static inline sycl::event solver(
        sycl::queue Q, gvars::mesh m,
        kern::data kdat,
        sycl::buffer<float,2> one,
        sycl::buffer<float,2> two,
        sycl::buffer<float,2> three,
        sycl::buffer<float,2> four,
        sycl::buffer<float,2> five,
        sycl::buffer<float,2> six){

            return Q.submit([&](sycl::handler& h){
                sycl::accessor one_acc{one,h} ;
                sycl::accessor two_acc{two,h} ;
                sycl::accessor three_acc{three,h} ;
                sycl::accessor four_acc{four,h} ;
                sycl::accessor five_acc{five,h} ;
                sycl::accessor six_acc{six,h} ;
                kern_func_class the_kern_operator(kdat, one_acc, two_acc, three_acc, four_acc, five_acc, six_acc) ;
                h.parallel_for(m.get_iter_range(), the_kern_operator );
            });
    }


    template<class kern_func_class, int N>
    static inline sycl::event bc(
        sycl::queue Q, gvars::mesh m,
        kern::data kdat,
        sycl::buffer<float,2> one,
        sycl::buffer<float,2> two){
            sycl::range<1> bc_range(m.alloc_range[N]) ;
            return Q.submit([&](sycl::handler& h){
                sycl::accessor one_acc{one,h} ;
                sycl::accessor two_acc{two,h} ;
                kern_func_class the_kern_operator(kdat, one_acc, two_acc) ;
                h.parallel_for(bc_range, the_kern_operator );
            });
    }

/*
    static inline sycl::event kobayashi_aniso(
        sycl::queue Q, gvars::mesh m,
        kern::data kdat,
        sycl::buffer<float,2> PHI_INP,
        sycl::buffer<float,2> PHI_OUT,
        sycl::buffer<float,2> TEMP_INP,
        sycl::buffer<float,2> TEMP_OUT){
            sycl::range<1> bc_range(m.alloc_range[0]) ; 
            Q.submit([&](sycl::handler& h){
                sycl::accessor phi_inp_acc{PHI_INP,h} ;
                sycl::accessor temp_inp_acc{TEMP_INP,h} ;
                h.parallel_for(bc_range, kern::bc::periodic(kdat, phi_inp_acc,temp_inp_acc));
            }).wait();

            return Q.submit([&](sycl::handler& h){
                sycl::accessor phi_inp_acc{PHI_INP,h} ;
                sycl::accessor phi_out_acc{PHI_OUT,h} ;
                sycl::accessor temp_inp_acc{TEMP_INP,h} ;
                sycl::accessor temp_out_acc{TEMP_OUT,h} ;
                h.parallel_for(m.get_iter_range(),kern::kobayashi_aniso(kdat, phi_inp_acc, phi_out_acc, temp_inp_acc, temp_out_acc));
            });
    }
*/


} // namespace enq
#endif