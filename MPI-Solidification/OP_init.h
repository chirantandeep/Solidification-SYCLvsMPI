class OP_init
{
private:
    int CENTER ;
    int RADIUS ;
    float FOREGROUND ;
    float BACKGROUND ;

public:
    OP_init(int c, int r, float fg, float bg){
        CENTER = c ;
        RADIUS = r ;
        FOREGROUND = fg ;
        BACKGROUND = bg ;

    }

    float init2(int j, int i){
        i -= CENTER ;
        j -= CENTER ;
        if (i*i + j*j <= RADIUS*RADIUS ){
            return FOREGROUND ;
        }else{
            return BACKGROUND ;
        }

    }
    
    float init(int j, int i){
        if (j < 10 ){
            return FOREGROUND ;
        }else{
            return BACKGROUND ;
        }

    }
};