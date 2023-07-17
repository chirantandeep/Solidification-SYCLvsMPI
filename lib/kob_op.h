#ifndef operatorsh
#define operatorsh

#include "pde_op.h"
#include "global.h"

namespace kobop{

static inline float theta (float FIELD[9]){
    float dFdx = ( FIELD[1] - FIELD[3] ) ;
    float dFdy = ( FIELD[2] - FIELD[4] ) ;
    if (dFdx*dFdx < 1e-10 ){
        return 1.57079 ;
    }else {
        return sycl::atan( (float)dFdy/dFdx ) ;
    }
}

static inline float theta(float dFdy, float dFdx){
    if (dFdx*dFdx < 1e-18 ){
        return 1.57079 ;
    }else {
        return sycl::atan( (float)dFdy/dFdx ) ;
    }
}

static inline float epsilon(kern::data dat, float theta){
    return (dat.kv.eps0 * (1.0 + dat.kv.delta * sycl::cos( dat.kv.J* (theta - dat.kv.theta0)))) ;
}

static inline float DepsilonDtheta(kern::data dat, float theta){
    return -dat.kv.eps0 * dat.kv.delta * dat.kv.J * sycl::sin( dat.kv.J* (theta - dat.kv.theta0) ) ;
}

static inline float epsDepsDtheta(kern::data dat, float dFdy, float dFdx){
    float th = theta(dFdy, dFdx) ;
    return epsilon(dat, th)*DepsilonDtheta(dat, th) ;
}

template<int N>
static inline float barrier_potential(gvars::barrier_potential_func_arrays pot_arr, float phi, float m){}

template< >
inline float barrier_potential<9>(gvars::barrier_potential_func_arrays pot_arr, float phi, float m){
    float val = 0.0 ;
    val += pot_arr.coeff[0]*sycl::pown(m,pot_arr.mm_pow[0])*sycl::pown(phi, pot_arr.phi_pow[0]) ;
    val += pot_arr.coeff[1]*sycl::pown(m,pot_arr.mm_pow[1])*sycl::pown(phi, pot_arr.phi_pow[1]) ;
    val += pot_arr.coeff[2]*sycl::pown(m,pot_arr.mm_pow[2])*sycl::pown(phi, pot_arr.phi_pow[2]) ;
    val += pot_arr.coeff[3]*sycl::pown(m,pot_arr.mm_pow[3])*sycl::pown(phi, pot_arr.phi_pow[3]) ;
    val += pot_arr.coeff[4]*sycl::pown(m,pot_arr.mm_pow[4])*sycl::pown(phi, pot_arr.phi_pow[4]) ;
    val += pot_arr.coeff[5]*sycl::pown(m,pot_arr.mm_pow[5])*sycl::pown(phi, pot_arr.phi_pow[5]) ;
    val += pot_arr.coeff[6]*sycl::pown(m,pot_arr.mm_pow[6])*sycl::pown(phi, pot_arr.phi_pow[6]) ;
    val += pot_arr.coeff[7]*sycl::pown(m,pot_arr.mm_pow[7])*sycl::pown(phi, pot_arr.phi_pow[7]) ;
    val += pot_arr.coeff[8]*sycl::pown(m,pot_arr.mm_pow[8])*sycl::pown(phi, pot_arr.phi_pow[8]) ;
    return val ;
}


}

#endif