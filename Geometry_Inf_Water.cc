//Geometry_Inf_Water.cc - Basic geometry handler. This simulates an infinite tank of water.
//
//This file essentially wraps a bounding box around some user-specified geometery.
//
//Programming notes:
//  -Do not make items here "const", because they will not show up when loading.
//  -Avoid using macro variables here because they will be obliterated during loading.
//  -Wrap dynamically-loaded code with extern "C", otherwise C++ compilation will mangle function names, etc.
//
// From man page for dlsym/dlopen:  For running some 'initialization' code prior to finishing loading:
// "Instead,  libraries  should  export  routines using the __attribute__((constructor)) and __attribute__((destructor)) function attributes.  See the gcc info pages for
//       information on these.  Constructor routines are executed before dlopen() returns, and destructor routines are executed before dlclose() returns."
//   ---for instance, we can use this to seed a random number generator with a random seed. However, in order to pass in a specific seed (and pass that seed to the library)
//      we need to define an explicitly callable initialization function. In general, these libraries should have both so that we can quickly adjust behaviour if desired.
//  

#include <iostream>
#include <string>
#include <vector>

#include <cmath>

#include "./Misc.h"
#include "./MyMath.h"

#include "./Constants.h" 
#include "./Structs.h"

#ifdef __cplusplus
    extern "C" {
#endif

std::string MODULE_NAME(__FILE__);
std::string FILE_TYPE("GEOMETRY");

double SMALLEST_FEATURE = 1.0;     //The smallest feature in the geometry - useful for transporting particles through a vacuum in a sensible way.
bool VERBOSE = false;

vec3<double> position(0.0, 0.0, 0.0); //The geometric location of the beam point 'spout.'


#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_geometry_inf_water.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_geometry_inf_water.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}

void set_position(const vec3<double> &in){
    position = in;
    return;
}

vec3<double> get_position(const struct Functions &Loaded_Funcs){
    return position;
}



//Given three clamped [0,1], random, uniformly-distributed numbers, we return a (three-vector) unit vector pointing in the direction 
// which a new beam particle will have.
//
//For instance, for an isotropic point source, we just return a random orientation. For a sharply directed beam, we can probably
// just return a constant, directed orientation. In between, we will likely have some angular distribution.
//
//NOTE: It would be better to start with a unit vector and rotate it twice --> no sqrt, only need two randoms.
//
vec3<double> get_orientation(const double &ina, const double &inb, const double &inc){
    const double shifteda = 2.0*ina - 1.0;
    const double shiftedb = 2.0*inb - 1.0;
    const double shiftedc = 2.0*inc - 1.0;
    const double tot = sqrt(shifteda*shifteda + shiftedb*shiftedb + shiftedc*shiftedc);
    return vec3<double>(shifteda/tot, shiftedb/tot, shiftedc/tot);
}


//Given a point in space, return the material which occupies the space.
// This is a bitmap-style bounding box arrangement. Alternatively, one
// could think of using a more vector-like approach.
unsigned char geometry_type(const vec3<double> &in){ 

    //We have an infinitely-long, infinitely-strong, infinitely-devoid of life fishtank in this case :).
    return Material::Water;
}


#ifdef __cplusplus
    }
#endif

