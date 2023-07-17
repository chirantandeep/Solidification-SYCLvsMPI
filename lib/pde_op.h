#ifndef pdeoperatorsh
#define pdeoperatorsh

#include "../src/compile_time_vars.h"

namespace pdeop{

template<int S>
static inline void get_stencil(sycl::accessor<float,2> FIELD, int y, int x, float stencil[S]){

}
template< >
inline void get_stencil<5>(sycl::accessor<float,2> FIELD, int y, int x, float stencil[5]){
    stencil[0] = FIELD[y][x] ;
    stencil[1] = FIELD[y][x+1] ;
    stencil[2] = FIELD[y+1][x] ;
    stencil[3] = FIELD[y][x-1] ;
    stencil[4] = FIELD[y-1][x] ;   
}

template< >
inline void get_stencil<9>(sycl::accessor<float,2> FIELD, int y, int x, float stencil[9]){
    stencil[0] = FIELD[y][x] ;
    stencil[1] = FIELD[y][x+1] ;
    stencil[2] = FIELD[y+1][x] ;
    stencil[3] = FIELD[y][x-1] ;
    stencil[4] = FIELD[y-1][x] ;
    stencil[5] = FIELD[y+1][x+1] ;
    stencil[6] = FIELD[y+1][x-1] ;
    stencil[7] = FIELD[y-1][x-1] ;
    stencil[8] = FIELD[y-1][x+1] ;
}


static inline float gradx (sycl::accessor<float,2> FIELD, int j, int i ){
    return (FIELD[j][i+1] - FIELD[j][i-1])/(2.0) ;
}

static inline float grady (sycl::accessor<float,2> FIELD, int j, int i ){
    return (FIELD[j+1][i] - FIELD[j-1][i])/(2.0) ;
}

template<int S>
static inline float operator_dot_product(const float OP[S], float F[S]) {}

template< >
inline float operator_dot_product<5>(const float OP[5], float F[5]) {
    float val =  OP[0]*F[0] + OP[1]*F[1] +OP[2]*F[2] +OP[3]*F[3] +OP[4]*F[4] ;
    return val ;
}

template< >
inline float operator_dot_product<9>(const float OP[9], float F[9]) {
    float val =  OP[0]*F[0] + OP[1]*F[1] +OP[2]*F[2] +OP[3]*F[3] +OP[4]*F[4] +OP[5]*F[5] +OP[6]*F[6] +OP[7]*F[7] +OP[8]*F[8]; 
    return val ;
}


static inline float dot_product(sycl::float2 F1, sycl::float2 F2){
    return F1[0]*F2[0] + F1[1]*F2[1] ;
}

static inline sycl::float2 grad(sycl::accessor<float,2> FIELD, int y, int x){
    sycl::float2 grad ;
    grad[0] = ( FIELD[y+1][x] - FIELD[y-1][x] ) ;
    grad[1] = ( FIELD[y][x+1] - FIELD[y][x-1] ) ;
    return grad ;
}


static inline float laplacian0(sycl::accessor<float,2> FIELD, int y, int x){
    float lap = 0.0 ;
    lap += FIELD[y][x+1] ;
    lap += FIELD[y][x-1] ;
    lap += FIELD[y+1][x] ;
    lap += FIELD[y-1][x] ;
    lap -= 4.0*FIELD[y][x] ;
    return  lap ;
}

inline float laplacian1(sycl::accessor<float,2> FIELD, int y, int x){
    float lap = 0.0 ;
    lap += FIELD[y][x+1] ;
    lap += FIELD[y][x-1] ;
    lap += FIELD[y+1][x] ;
    lap += FIELD[y-1][x] ;
    lap += FIELD[y+1][x+1] ;
    lap += FIELD[y+1][x-1] ;
    lap += FIELD[y-1][x+1] ;
    lap += FIELD[y-1][x-1] ;
    lap -= 8.0*FIELD[y][x] ;
    return  lap ;
}

static inline float laplacian2(sycl::accessor<float,2> FIELD, int y, int x){
    float lap = 0.0 ;
    lap += FIELD[y][x+1] ;
    lap += FIELD[y][x-1] ;
    lap += FIELD[y+1][x] ;
    lap += FIELD[y-1][x] ;
    lap += 4.0*FIELD[y+1][x+1] ;
    lap += 4.0*FIELD[y+1][x-1] ;
    lap += 4.0*FIELD[y-1][x+1] ;
    lap += 4.0*FIELD[y-1][x-1] ;
    lap -= 20.0*FIELD[y][x] ;
    return  lap ;
}

static inline float laplacian(sycl::local_accessor<float,2> FIELD, int y, int x){
    float lap = 0.0 ;
    lap += FIELD[y][x+1] ;
    lap += FIELD[y][x-1] ;
    lap += FIELD[y+1][x] ;
    lap += FIELD[y-1][x] ;
    lap += FIELD[y+1][x+1] ;
    lap += FIELD[y+1][x-1] ;
    lap += FIELD[y-1][x+1] ;
    lap += FIELD[y-1][x-1] ;
    lap -= 8.0*FIELD[y][x] ;
    return  lap ;
}

static inline sycl::float2 normal_vector(sycl::accessor<float,2> FIELD, int y, int x){
    sycl::float2 nvec ;
    nvec[0] = ( FIELD[y+1][x] - FIELD[y-1][x] )/(2.0) ;
    nvec[1] = ( FIELD[y][x+1] - FIELD[y][x-1] )/(2.0) ;
    return nvec ;
}
}

#endif