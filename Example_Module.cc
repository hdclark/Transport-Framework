//Example_Module.cc - Basic, non-interactive module for instructional purposes.
// For a more advanced module example, look at Photons.cc, Electrons.cc, or similar.
//
//Programming notes:
//  -Do not make items here "const", because they will not show up when loading.
//  -Avoid using macro variables here because they will be obliterated during loading.
//  -Wrap dynamically-loaded code with extern "C", otherwise C++ compilation will mangle function names, etc.

#include <iostream>
#include <string>
#include <vector>

#include <cmath>

#ifdef __cplusplus
    extern "C" {
#endif

std::string MODULE_NAME(__FILE__);

std::string PARTICLE_TYPE("Photon");

int twelve = 12;


void some_void_function(void){
    std::cout << "The name of this function is \"" << __PRETTY_FUNCTION__ ;
    std::cout << "\" and it lives in the file \"" << __FILE__ << "\"" << std::endl;
    return;
}

double some_double_function(double a,double b){
    std::cout << "The two numbers received were " << a << " and " << b << std::endl;
    return cos(a);
}

std::vector<double> some_vector_function( double *a ){
    std::vector<double> outgoing;
    outgoing.push_back(*a);
    return outgoing;
}

#ifdef __cplusplus
    }
#endif

