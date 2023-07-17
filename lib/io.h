#ifndef iofileh
#define iofileh

#include "../src/compile_time_vars.h"
#include "global.h"

namespace io{

static inline float get_float_from_string(std::string var_str){
    std::regex percent_match("(.*)%.*");
    std::smatch matched ; 
    float val ;
    if(std::regex_match(var_str, matched, percent_match)){
        std::cout << "% detected : " << matched[0].str() << " :: " ;
        val = stof(matched[1].str())/100 ;
        std::cout << "value = " << val << "\n" ;
        return val ; 
    }else{
        std::cout << "abs value detected : " << var_str << " :: " ;
        val = stof(var_str) ;
        std::cout << "value = " << val << "\n" ;
        return val;
    }
}

static inline gvars::init read_init_file(std::string filename = "./inp/init.vars"){
    gvars::init iv ;

    std::ifstream infile(filename) ;
    std::string line, variable ;
    
    std::regex rectangle_match("^set rectangle = \\{(.*),(.*),(.*),(.*)\\}$") ;
    std::regex circle_match("^set circle = \\{(.*),(.*),(.*)\\}$") ;
    
    std::smatch matched, matched2 ; 

    std::cout << "readig file : " << filename << "\n" ;
    while(std::getline(infile, line)){
        if(std::regex_match(line, matched,rectangle_match)){
            iv.style = "rectangle" ;
            std::cout << "line matched : " << matched[0].str() << "\n";
            iv.pos_y  =  get_float_from_string(matched[1].str()) ;
            iv.pos_x  =  get_float_from_string(matched[2].str()) ;
            iv.size_y =  get_float_from_string(matched[3].str()) ;
            iv.size_x =  get_float_from_string(matched[4].str()) ;
        }else if(std::regex_match(line, matched,circle_match)){
            iv.style = "circle" ;
            std::cout << "line matched : " << matched[0].str() << "\n";
            iv.pos_y =  get_float_from_string(matched[1].str()) ;
            iv.pos_x =  get_float_from_string(matched[2].str()) ;
            iv.size_y  =  get_float_from_string(matched[3].str()) ;
            iv.size_x = iv.size_y ;
        }
    }
    return iv ;
}

static inline gvars::mesh read_mesh_vars(std::string filename = "./inp/mesh.vars" ) {

    std::ifstream infile(filename) ;
    std::string line ;
    gvars::mesh pfm ; 
    std::string varname, value ; 
    char equalsign, semicolon ;
    std::cout << "--------------------------------------" << "\n";
    std::cout << "reading mesh variables from " << filename << "\n";

    while(std::getline(infile,line)){
        if(line[0] != '#'){
            std::istringstream iss(line);
            iss >> varname >> equalsign >> value >> semicolon ; 
            if(varname == "MESH_X"){
                pfm.iter_range[1] = stoi(value) ;
                std::cout << "MESH_X: " << pfm.iter_range[1] << "\n" ; 
            }
            else if(varname == "MESH_Y"){
                pfm.iter_range[0] = stoi(value) ;
                std::cout << "MESH_Y: " << pfm.iter_range[0] << "\n" ;
            }
            else if(varname == "DELTA_X"){
                pfm.deltax = stof(value) ;
                std::cout << "DELTA_X: " << pfm.deltax << "\n" ;
            }
            else if(varname == "DELTA_Y"){
                pfm.deltay = stof(value) ;
                std::cout << "DELTA_Y: " << pfm.deltay << "\n" ;
            }
            else if(varname == "DELTA_T"){
                pfm.deltat = stof(value) ;
                std::cout << "DELTA_T: " << pfm.deltat << "\n" ;
            }
            else if(varname == "OFFSET"){
                pfm.offset = {stoi(value), stoi(value)} ;
                std::cout << "OFFSET: {" << pfm.offset[0] << ","<<pfm.offset[1] << "}\n" ;
            }
        }
    }
    
    pfm.alloc_range = pfm.iter_range + 2*pfm.offset ;
    std::cout << "ALLOC_RANGE: {" << pfm.alloc_range[0] << ","<<pfm.alloc_range[1] << "}\n" ;
    std::cout << "--------------------------------------" << "\n";
    
    infile.close();
    return pfm ;
}

static inline gvars::kob read_kobayashi_vars(std::string filename = "./inp/kobayashi.vars") {

    std::ifstream infile(filename) ;
    std::string line ;
    gvars::kob pfm ; 
    std::string varname, value ; 
    char equalsign, semicolon ;
    std::cout << "--------------------------------------" << "\n";
    std::cout << "reading kobayashi variables from " << filename << "\n";

    while(std::getline(infile,line)){
        if(line[0] != '#' && line.length() >= 3){

            std::istringstream iss(line);
            iss >> varname >> equalsign >> value >> semicolon ; 
            if(varname == "PI"){
                pfm.pi = stof(value) ;
                std::cout << "PI: " << value << "\n" ;
            }
            else if(varname == "EPS0"){
                pfm.eps0 = stof(value) ;
                std::cout << "EPS0: " << value << "\n" ;
            }
            else if(varname == "ALPHA"){
                pfm.alpha = stof(value) ;
                std::cout << "ALPHA: " << value << "\n" ;
            }
            else if(varname == "GAMMA"){
                pfm.gamma = stof(value) ;
                std::cout << "GAMMA: " << value << "\n" ;
            }
            else if(varname == "DELTA"){
                pfm.delta = stof(value) ;
                std::cout << "DELTA: " << value << "\n" ;
            }
            else if(varname == "J"){
                pfm.J = stof(value) ;
                std::cout << "J: " << value << "\n" ;
            }
            else if(varname == "TAU"){
                pfm.tau = stof(value) ;
                std::cout << "TAU: " << value << "\n" ;
            }
            else if(varname == "THETA0"){
                pfm.theta0 = stof(value) ;
                std::cout << "THETA0: " << value << "\n" ;
            }
            else if(varname == "DIFF_COEFF"){
                pfm.diff_coeff = stof(value) ;
                std::cout << "DIFF_COEFF: " << value << "\n" ;
            }
            else if(varname == "LATENT_HEAT"){
                pfm.latent_heat = stof(value) ;
                std::cout << "LATENT_HEAT: " << value << "\n" ;
            }
            else if(varname == "SOLID"){
                pfm.solid = stof(value) ;
                std::cout << "SOLID: " << value << "\n" ;
            }
            else if(varname == "LIQUID"){
                pfm.liquid = stof(value) ;
                std::cout << "LIQUID: " << value << "\n" ;
            }
            else if(varname == "TEMP_SOLID"){
                pfm.temp_sol = stof(value) ;
                std::cout << "TEMP_SOLID: " << value << "\n" ;
            }
            else if(varname == "TEMP_LIQUID"){
                pfm.temp_liq = stof(value) ;
                std::cout << "TEMP_LIQUID: " << value << "\n" ;
            }
        }
    }
    std::cout << "--------------------------------------" << "\n";
    
    infile.close();
    return pfm ;
}

static inline gvars::control read_control_vars(std::string filename = "./inp/control.vars") {

    std::ifstream infile(filename) ;
    std::string line ;
    gvars::control pfm ; 
    std::string varname, value ; 
    char equalsign, semicolon ;
    std::cout << "--------------------------------------" << "\n";
    std::cout << "reading control variables from " << filename << "\n";

    while(std::getline(infile,line)){
        if(line[0] != '#' && line.length() >= 3){
            std::istringstream iss(line);
            iss >> varname >> equalsign >> value >> semicolon ; 
            if(varname == "NTIMESTEPS"){
                pfm.iterations = stoi(value) ;
                std::cout << "NTIMESTEPS: " << pfm.iterations << "\n" ;
            }
            else if(varname == "NSAVE"){
                pfm.nsave = stoi(value) ;
                std::cout << "NSAVE: " << value << "\n" ;
            }
            else if(varname == "NOISE_AMP"){
                pfm.noise_amp = stof(value) ;
                std::cout << "NOISE_AMP: " << value << "\n" ;
            }
            else if(varname == "NOISE_FREQ"){
                pfm.noise_freq = stof(value) ;
                std::cout << "NOISE_FREQ: " << value << "\n" ;
            }
            else if(varname == "SAVE_AS"){
                pfm.saveas = value ;
                std::cout << "SAVE_AS: " << value << "\n" ;
            }
        }
    }
        std::cout << "--------------------------------------" << "\n";
    infile.close();
    return pfm ;
}

static inline gvars::operator_arrays read_operator_arrays(std::string filename = "./inp/operator.styles") {

    std::ifstream infile(filename) ;
    std::string line ;
    gvars::operator_arrays op_vars ; 
    std::string tmp, val ;
    std::cout << "--------------------------------------" << "\n";
    std::cout << "reading field operators from " << filename << "\n" ;
    
    std::regex lap_match("^set laplacian .*");
    //std::regex gradx_match("^@@ gradientx .*");
    //std::regex grady_match("^@@ gradienty .*");

    while(std::getline(infile,line)){
        std::istringstream iss(line);
        if(std::regex_match(line,lap_match)){
            std::cout << "line matched : " << line << "\n" ;
            iss >> tmp >> tmp ;
            for(int i = 0; i < CT_VARS::STENCIL_SIZE; i++){
                iss >> val ; 
                op_vars.laplacian[i] = stof(val) ;
                std::cout << "lap[" << i << "] = " << val << "\n" ;
            }
        }
        /*
        else if(std::regex_match(line,gradx_match)){
            iss >> tmp >> tmp ;
            for(int i = 0; i < CT_VARS::STENCIL_SIZE; i++){
                iss >> val ; 
                op_vars.gradx_array[i] = stof(val) ;
            }
        }else if(std::regex_match(line,grady_match)){
            iss >> tmp >> tmp ;
            for(int i = 0; i < CT_VARS::STENCIL_SIZE; i++){
                iss >> val ; 
                op_vars.grady_array[i] = stof(val) ;
            }
        }*/
    }
    std::cout << "--------------------------------------" << "\n";
    infile.close();
    return op_vars ;
}


static inline gvars::barrier_potential_func_arrays read_barrier_potential_func(std::string filename = "./inp/potential.styles") {

    std::ifstream infile(filename) ;
    std::string line ;
    gvars::barrier_potential_func_arrays pot_vars ; 
    std::string tmp, val, term ;
    std::cout << "--------------------------------------" << "\n";
    std::cout << "reading barrier potential from " << filename << "\n" ;
    
    //std::regex terms_match("^@@ MAX_BARRIER_POTENTIAL_TERMS = .*");
    std::regex potential_match("^set potential.*");
    std::regex term_match("^(.*)\\*mm\\^(.*)\\*phi\\^(.*)");
    std::smatch matched ;

    while(std::getline(infile,line)){
        std::istringstream iss(line);
        //std::cout<< line ; 
        if(std::regex_match(line,potential_match)){
            std::cout << "line matched : " << line << "\n"; 
            iss >> tmp >> tmp; // @@ and potential gone
            iss >> val ;
            //std::cout << "terms : " << val<< "\n" ;
            int i = 0 ;
            for(; i < stoi(val) ; i++){
                iss >> term ;
                if(std::regex_match(term, matched,term_match)){
                    std::cout << "term matched : " << term << "\n";
                    pot_vars.coeff[i]   = stof(matched[1].str()) ;
                    pot_vars.mm_pow[i]  = stoi(matched[2].str()) ;
                    pot_vars.phi_pow[i] = stoi(matched[3].str()) ;                    
                }
            }
            for(int k = i ; k < CT_VARS::MAX_BARRIER_POTENTIAL_TERMS; k++){
                pot_vars.coeff[k]   = 0 ;
                pot_vars.mm_pow[k]  = 0 ;
                pot_vars.phi_pow[k] = 0 ;    
            }
        }
    }

    std::cout << "potential function : \n" ;
    for(int i = 0 ; i < CT_VARS::MAX_BARRIER_POTENTIAL_TERMS; i++){
        std::cout << "coeff=" << pot_vars.coeff[i] << " mm_pow=" << pot_vars.mm_pow[i] << " phi_pow=" << pot_vars.phi_pow[i]<< "\n" ;
    }
    std::cout << "--------------------------------------" << "\n";

    infile.close();
    return pot_vars ;
}


void save2csv(gvars::mesh pfm, sycl::buffer<float,2> field, std::string filename){
    std::string full_filename ;
    full_filename = "./out/" + filename + ".csv" ;
    std::ofstream csvfile(full_filename) ;

    auto host_acc = field.get_access<sycl::access::mode::read>() ; 

    for (int y = 0 ; y<pfm.alloc_range[0] ; y++ ){
        for (int x = 0 ; x<pfm.alloc_range[1] ; x++ ){
            csvfile << std::scientific <<  host_acc[y][x] ;
            if ( x != (pfm.alloc_range[1] -1) ){
                csvfile << "," ;
            }
        }
        if ( y != (pfm.alloc_range[0] -1) ){
            csvfile << "\n" ;
        }
    }
    csvfile.close() ;
}

void save2vtk(gvars::mesh pfm, sycl::buffer<float,2> field, std::string filename){

    std::string full_filename ;
    full_filename = "./out/"  + filename + ".vtk" ;
    std::ofstream vtkfile(full_filename) ;

    auto host_acc = field.get_access<sycl::access::mode::read>() ;

    vtkfile <<  "# vtk DataFile Version 3.0\n" ;
    vtkfile << filename << "_fields\n";
    vtkfile << "ASCII\n" ;
    vtkfile << "DATASET STRUCTURED_POINTS\n" ;
    vtkfile << "DIMENSIONS " << pfm.alloc_range[1] << " " << pfm.alloc_range[0] <<" 1\n" ;
    vtkfile << "ORIGIN 0 0 0\n" ;
    vtkfile << std::scientific << "SPACING "<< pfm.deltax << " " << pfm.deltay << " 1.000000e+00\n";
    vtkfile << "POINT_DATA " << std::fixed << pfm.alloc_range[1]*pfm.alloc_range[0] << "\n" ;
    vtkfile << "SCALARS FCC double 1\nLOOKUP_TABLE default\n" ;
    int x = 0; 
    int y = 0;
    for(int i = 0; i < pfm.alloc_range[1]*pfm.alloc_range[0] ; i++){
        y = (int) i / pfm.alloc_range[1] ; 
        x = (int) i % pfm.alloc_range[1] ; 
        vtkfile << std::setprecision(2) << host_acc[y][x] << "\n" ;
    }

    vtkfile.close();
}

void save(std::string type, gvars::mesh pfm, sycl::buffer<float,2> field, std::string filename){
    if(type == "vtk"){
        save2vtk(pfm, field,filename);
    }else if(type == "csv"){
        save2csv(pfm,field,filename);
    }
    std::cout << "io>> saving file " << filename << "\n";
}


}
#endif