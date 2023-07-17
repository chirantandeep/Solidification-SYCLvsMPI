#ifndef iteratorsh
#define iteratorsh

#include "variables.h"
#include "../lib/OP.h"

class Iterator
{
private:
    sycl::int3 iterate_range ; 
public:
    Iterator(sycl::int3 RANGE3D) ;
    Iterator(size_t RZ, size_t RY, size_t RX );
    
    void run_kernel(
        sycl::queue Q,
        OrderParameter OP,
        FTYPES::KERNEL_FUCTION kernel_function) ;
};

inline Iterator::Iterator(sycl::int3 RANGE3D){
    iterate_range = RANGE3D ; 
}
inline Iterator::Iterator(size_t RZ, size_t RY, size_t RX)
{
    iterate_range[0] = RZ ;
    iterate_range[1] = RY ;
    iterate_range[2] = RX ;
}



void Iterator::run_kernel(
    sycl::queue Q,
    OrderParameter OP,
    FTYPES::KERNEL_FUCTION kernel_function
    ){

    sycl::range<3> alloc_range  (OP.vars.MESH_Z,   OP.vars.MESH_Y,   OP.vars.MESH_X) ;
    sycl::range<3> stencil_range(iterate_range[0], iterate_range[1], iterate_range[2]) ;
    
    sycl::buffer<float,3> buff_now(OP.field_now.data()->data()->data() , alloc_range) ;
    sycl::buffer<float,3> buff_next(OP.field_next.data()->data()->data() , alloc_range) ;

    Q.submit([&](sycl::handler& h){
        sycl::accessor FIELD_NOW{buff_now,h} ;
        sycl::accessor FIELD_NEXT{buff_next,h} ;

        h.parallel_for(stencil_range, [=](sycl::id<3> ID){
            FTYPES::kernfunc1(ID, FIELD_NOW, FIELD_NEXT) ;
        }) ;

    }).wait();


}

#endif