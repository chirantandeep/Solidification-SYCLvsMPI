#ifndef trialhfuncs
#define trialhfuncs

namespace trial
{

   class laplacian
    {
    public:
        laplacian(
            pfm_vars vars,
            sycl::accessor<float,2> in_acc,
            sycl::accessor<float,2> out_acc) :
            pfm(vars),
            in_field(in_acc),
            out_field(out_acc) {}

        void operator()(sycl::id<2> ID) const {
            int y = ID[0] + pfm.offset[0] ; 
            int x = ID[1] + pfm.offset[1] ; 

            float lap = op::laplacian1(in_field,y,x) ; 
            out_field[y][x] = pfm.deltat*(lap)/(pfm.deltax*pfm.deltay) ;
        }
    private :
        pfm_vars pfm ;
        sycl::accessor<float,2> in_field ;
        sycl::accessor<float,2> out_field ;
    };

static inline float theta (sycl::accessor<float,2> FIELD, int j, int i){
    float dFdx = ( FIELD[j][i+1] - FIELD[j][i-1] ) ;
    float dFdy = ( FIELD[j+1][i] - FIELD[j-1][i] ) ;
    if (dFdx*dFdx < 1e-10 ){
        return 1.57079 ;
    }else {
        return sycl::atan( (float)dFdy/dFdx ) ;
    }
}


static inline float phase_evol_rhs_term1(pfm_vars pfm, sycl::accessor<float,2> FIELD, int j,int i){

    float tmp1, tmp2, term11, term12 ;
    tmp1 = pdeop::gradx(FIELD,j+1,i)*epsDepsDtheta(pfm, FIELD,j+1,i) ;
    tmp2 = pdeop::gradx(FIELD,j-1,i)*epsDepsDtheta(pfm, FIELD,j-1,i) ;
    term11 = (tmp1 - tmp2)/(pfm.deltay*pfm.deltax) ;

    tmp1 = pdeop::grady(FIELD,j,i+1)*epsDepsDtheta(pfm, FIELD,j,i+1) ;
    tmp2 = pdeop::grady(FIELD,j,i-1)*epsDepsDtheta(pfm, FIELD,j,i-1) ;
    term12 = (tmp1 - tmp2)/(pfm.deltax*pfm.deltay) ;

    return (term11 - term12) ;
}

static inline float epsDepsDtheta(pfm_vars pfm, sycl::accessor<float,2> FIELD, int j,int i){
    float th = theta(FIELD,j,i) ;
    return epsilon(pfm, th)*DepsilonDtheta(pfm, th) ;
}

template<class KERN>
void enqueue_kernel(sycl::queue Q, OrderParameter OP, KERN kernel){
    
    auto ar = OP.var.alloc_range ;
    auto ir = OP.var.iter_range ;

    sycl::range<3> arange(ar[0], ar[1], ar[2]) ; 
    sycl::buffer<float,3> buff(OP.vec.data(), arange) ;

    Q.submit([&](sycl::handler& h){
        sycl::accessor acc{buff,h} ;
        auto IR = sycl::range<3>(ir[0], ir[1], ir[2]) ;
        h.parallel_for(IR,[=](sycl::id<3> ID){

            //kernel(ID,acc);
            int x = ID[1] ;
            float val = 0.0 ;
            val += ifunc1(x) ;
            acc[ID] = val ; 
        });
    });

}

} // namespace trial



#endif