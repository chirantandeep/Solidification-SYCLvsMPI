#ifndef phasefieldmodeloph
#define phasefieldmodeloph

namespace pfmop{
    float constexpr EPS4 = 0.001 ; 


static inline float anisotropy_function(sycl::accessor<float,2> FIELD, int y, int x){
    float dFdx, dFdy,grad2F, anisofun ;
    dFdx = (FIELD[y][x+1] - FIELD[y][x-1])/(2.0) ;
    dFdy = (FIELD[y+1][x] - FIELD[y-1][x])/(2.0) ;
    grad2F = dFdx*dFdx + dFdy*dFdy ;

    if(grad2F < 0.01 ){
        return 0.0 ;
    }else{
        anisofun  = ( dFdx*dFdx*dFdx*dFdx + dFdy*dFdy*dFdy*dFdy ) / (grad2F*grad2F) ;
        return anisofun;
    }
}

static inline float anisotropy_function2(sycl::accessor<float,2> FIELD, int y, int x){
    float dFdx, dFdy,grad2F, anisofun ;
    dFdx = (FIELD[y][x+1] - FIELD[y][x-1])/(2.0) ;
    dFdy = (FIELD[y+1][x] - FIELD[y-1][x])/(2.0) ;
    grad2F = dFdx*dFdx + dFdy*dFdy ;

    if(grad2F < 0.01 ){
        return 0.0 ;
    }else{
        anisofun  = ( dFdx*dFdx*dFdx*dFdx + dFdy*dFdy*dFdy*dFdy ) / (grad2F*grad2F) ;
        anisofun = (1.0 - 3.0*EPS4)*(1.0 + (4.0*EPS4 / (1.0 - 3.0*EPS4))*anisofun ) ;
        return anisofun;
    }
}

static inline bool check_neighbors(sycl::accessor<float,2> FIELD, int j, int i){
    bool nbh = true ;
    float c = FIELD[j][i] ;
    nbh = nbh && (FIELD[j+1][i] == c) ; 
    nbh = nbh && (FIELD[j-1][i] == c) ; 
    nbh = nbh && (FIELD[j][i+1] == c) ; 
    nbh = nbh && (FIELD[j][i-1] == c) ;
    return false ;
}

}

#endif