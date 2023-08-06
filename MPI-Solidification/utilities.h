#include <bits/stdc++.h>

namespace utilities{

    class writer
    {
    private:
        int offset ;
        int yrange ;
        int xrange ;
    public:
        writer(int o, int yr, int xr){
            offset = o ;
            yrange = yr ;
            xrange = xr ;
        };
        
        template<typename T>
        void write(T ARR, std::ofstream &outstream ){
            outstream.seekp(offset) ;
            for (int j = 1 ; j < yrange; j++){
                for (int i = 1; i < xrange; i ++){
                    outstream << std::scientific << (float)ARR[j][i] ;
                    if ( i == xrange -1 ){
                        outstream << "\n" ;
                    }
                    else {
                        outstream << "," ;
                    }
                }
            }
        }


    };    





}