namespace KOB{
    float DX = 0.03 ;
    float DY = DX ;
    float DT = 1e-4 ;
    float EPS_BAR = 0.01 ;
    float ALPHA = 0.92 ;
    float GAMMA = 10 ;
    float DELTA = 0.2 ; // 0.6
    float J = 4 ;
    float TAU = 3.0e-4 ;
    float THETA0 = 0.01  ;
    float THERMAL_DIFFUSIVITY = 1.0 ;
    float LATENT_HEAT_SLD = 1.32 ; //1.32 ;

    float const LIMIT_ZERO = 1e-20 ;

    template<typename TYPE>
    static inline bool check_neighbors(TYPE FIELD, int j, int i){
        bool nbh = true ;
        float c = FIELD[j][i] ;
        nbh = nbh && (FIELD[j+1][i] == c) && (FIELD[j-1][i] == c)&& (FIELD[j][i+1] == c)&& (FIELD[j][i-1] == c) ;
        return nbh ;
    }

    template<typename TYPE>
    static inline float laplacian (TYPE FIELD, int j, int i ){
        float lap = 0.0 ;
        lap += FIELD[j][i+1] ;
        lap += FIELD[j+1][i] ;
        lap += FIELD[j][i-1] ;
        lap += FIELD[j-1][i] ;

        lap -= 4.0*FIELD[j][i] ;

        return lap/(DX*DY) ; 
    }

    template<typename TYPE>
    static inline float gradx (TYPE FIELD, int j, int i ){
        return (FIELD[j][i+1] - FIELD[j][i-1])/(2.0*DX) ;
    }

    template<typename TYPE>
    static inline float grady (TYPE FIELD, int j, int i ){
        return (FIELD[j+1][i] - FIELD[j-1][i])/(2.0*DY) ;
    }

    template<typename TYPE>
    static inline float theta (TYPE FIELD, int j, int i){
        float dFdx = ( FIELD[j][i+1] - FIELD[j][i-1] ) ;
        float dFdy = ( FIELD[j+1][i] - FIELD[j-1][i] ) ;
        if (dFdx == 0.0  ){
            return 1.57079 ;
        }else {
            return atan( dFdy/dFdx ) ;
        }
    }

    static inline float epsilon(float theta){
        return (EPS_BAR * (1.0 + DELTA * cos( J* (theta - THETA0)))) ;
    }

    static inline float DepsilonDtheta(float theta){
        return -EPS_BAR * DELTA * J * sin( J* (theta - THETA0) ) ;
    }

    template<typename TYPE>
    static inline float epsDepsDtheta(TYPE FIELD, int j,int i){
        float th = theta(FIELD,j,i) ;
        return epsilon(th)*DepsilonDtheta(th) ;
    }


    template<typename TYPE>
    static inline float phase_evol_rhs_term1(TYPE FIELD, int j,int i){

        float tmp1, tmp2, term11, term12 ;
        tmp1 = gradx(FIELD,j+1,i)*epsDepsDtheta(FIELD,j+1,i) ;
        tmp2 = gradx(FIELD,j-1,i)*epsDepsDtheta(FIELD,j-1,i) ;
        term11 = (tmp1 - tmp2)/(2.0*DY) ;

        tmp1 = grady(FIELD,j,i+1)*epsDepsDtheta(FIELD,j,i+1) ;
        tmp2 = grady(FIELD,j,i-1)*epsDepsDtheta(FIELD,j,i-1) ;
        term12 = (tmp1 - tmp2)/(2.0*DX) ;

        return (term11 - term12) ;

    }



}