#ifndef boundaryconditionsh
#define boundaryconditionsh

#include "global.h"

namespace kern{
namespace bc{

    class periodic_y
    {
    private :
        data kdat ;
        sycl::accessor<float,2> phi  ;
        sycl::accessor<float,2> temp ;

    public:
        periodic_y(
            data vars,
            sycl::accessor<float,2> one,
            sycl::accessor<float,2> three) :
            kdat(vars),
            phi(one),
            temp(three) {}

        void operator()(sycl::id<1> ID) const {
            int ylim = kdat.ar[0] -1 ;
            for(int off = 0; off < kdat.off[0]; off++){
                phi[off][ID] = phi[ylim-1-off][ID] ;
                phi[ylim-off][ID] = phi[off+1][ID] ;
                temp[off][ID] = temp[ylim-1-off][ID] ;
                temp[ylim-off][ID] = temp[off+1][ID] ;
            }
        }
    };

    class periodic_x
    {
    private :
        data kdat ;
        sycl::accessor<float,2> phi  ;
        sycl::accessor<float,2> temp ;

    public:
        periodic_x(
            data vars,
            sycl::accessor<float,2> one,
            sycl::accessor<float,2> three) :
            kdat(vars),
            phi(one),
            temp(three) {}

        void operator()(sycl::id<1> ID) const {
            int xlim = kdat.ar[1] -1 ;
            for(int off = 0; off < kdat.off[0]; off++){
                phi[ID][off] = phi[ID][xlim-1-off] ;
                phi[ID][xlim-off] = phi[ID][off+1] ;
                temp[ID][off] = temp[ID][xlim-1-off] ;
                temp[ID][xlim-off] = temp[ID][off+1] ;
            }
        }
    };


}
}


#endif