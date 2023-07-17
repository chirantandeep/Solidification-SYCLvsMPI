#ifndef dynamicreafh
#define dynamicreafh

#include "global.h"

namespace dynamic_read {

static inline bool is_run_command( std::string line, gvars::control * cvars){
    std::istringstream iss(line);
    std::string code_word, expression ;
    iss >> code_word >> expression ;
    if (code_word == "run"){
        cvars->iterations = cvars->iterations + stoi(expression) ;
        std::cout << "main>> runing " << cvars->iterations << " iterations\n" ;
        return true ;
    }else{
        return false ;
    }

}

static inline bool is_set_command(std::string line, gvars::kob * kob, gvars::control * cvars){
    std::string command, varname, value ; 
    char equalsign ;
    std::istringstream iss(line);
    iss >> command >> varname >> equalsign >> value ; 
    if (command == "set"){
        if(varname == "EPS0"){
            kob->eps0 = stof(value) ;
            std::cout << "EPS0: " << value << "\n" ;
        }
        else if(varname == "ALPHA"){
            kob->alpha = stof(value) ;
            std::cout << "ALPHA: " << value << "\n" ;
        }
        else if(varname == "GAMMA"){
            kob->gamma = stof(value) ;
            std::cout << "GAMMA: " << value << "\n" ;
        }
        else if(varname == "DELTA"){
            kob->delta = stof(value) ;
            std::cout << "DELTA: " << value << "\n" ;
        }
        else if(varname == "J"){
            kob->J = stof(value) ;
            std::cout << "J: " << value << "\n" ;
        }
        else if(varname == "TAU"){
            kob->tau = stof(value) ;
            std::cout << "TAU: " << value << "\n" ;
        }
        else if(varname == "THETA0"){
            kob->theta0 = stof(value) ;
            std::cout << "THETA0: " << value << "\n" ;
        }
        else if(varname == "DIFF_COEFF"){
            kob->diff_coeff = stof(value) ;
            std::cout << "DIFF_COEFF: " << value << "\n" ;
        }
        else if(varname == "LATENT_HEAT"){
            kob->latent_heat = stof(value) ;
            std::cout << "LATENT_HEAT: " << value << "\n" ;
        }
        else if(varname == "SAVE_FREQ"){
            cvars->nsave = stoi(value) ;
            std::cout << "SAVE_FREQ: " << value << "\n" ;
        }
        else if(varname == "NOISE_AMP"){
            cvars->noise_amp = stof(value) ;
            std::cout << "NOISE_AMP: " << value << "\n" ;
        }
        else if(varname == "NOISE_FREQ"){
            cvars->noise_freq = stof(value) ;
            std::cout << "NOISE_FREQ: " << value << "\n" ;
        }else {
            std::cout << "varname not recognized\n" ;
        }
        return true ;
    } else{
        return false ;
    }
}

}
#endif
