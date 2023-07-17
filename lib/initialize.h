#ifndef initfuncsh
#define initfuncsh

#include "global.h"

namespace initialize{


    sycl::buffer<float,2> field(gvars::mesh pfm){
        auto buff = sycl::buffer<float,2>(pfm.get_alloc_range()) ;
        auto acc = buff.get_access<sycl::access::mode::write>() ;
        for(int i = 0; i < pfm.alloc_range[0]; i++){
            for(int j = 0; j < pfm.alloc_range[1]; j++){
                acc[i][j] = 0.0 ; 
            }
        }
        return buff ; 
    }

    sycl::buffer<float,2> field(gvars::mesh pfm, gvars::init iv, float fg, float bg){
        auto buff = sycl::buffer<float,2>(pfm.get_alloc_range()) ;
        auto acc = buff.get_access<sycl::access::mode::write>() ;

        int ceny, cenx ; 
        ceny = iv.pos_y <= 1.0 ? iv.pos_y * pfm.alloc_range[0] : iv.pos_y ; 
        cenx = iv.pos_x <= 1.0 ? iv.pos_x * pfm.alloc_range[1] : iv.pos_x ;
        int sy, sx ;
        sy = iv.size_y <= 1.0 ? iv.size_y * pfm.alloc_range[0] : iv.size_y ; 
        sx = iv.size_x <= 1.0 ? iv.size_x * pfm.alloc_range[1] : iv.size_x ;

        int y, x ,i,j ;
        if(iv.style == "rectangle"){
            for(i = 0; i < pfm.alloc_range[0]; i++){
                for(j = 0; j < pfm.alloc_range[1]; j++){
                    y = i-ceny ;
                    x = j-cenx ;
                    acc[i][j] = x*x < sx*sx && y*y < sy*sy ? fg : bg ; 
                }
            }
            return buff ; 
        }else if(iv.style == "circle"){
            for(i = 0; i < pfm.alloc_range[0]; i++){
                for(j = 0; j < pfm.alloc_range[1]; j++){
                    y = i-ceny ;
                    x = j-cenx ;
                    acc[i][j] = x*x + y*y < sx*sy ? fg : bg ; 
                }
            }
            return buff ; 
        }else{
            for(int i = 0; i < pfm.alloc_range[0]; i++){
                for(int j = 0; j < pfm.alloc_range[1]; j++){
                    acc[i][j] = 0.0 ; 
                }
            }
            return buff ; 
        }
    }

}


#endif