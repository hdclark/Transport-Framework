//Geometry_Water_Tank.cc - Simulates a rectangular tank of water. Designed for a beam above z=0 plane to point downward.
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

bool VERBOSE = false;
double SMALLEST_FEATURE = 1.0;     //The smallest feature in the geometry - useful for transporting particles through a vacuum in a sensible way.

vec3<double> position(0.0, 0.0, 0.0); //The geometric location of the center of the source's spout.


#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_geometry_water_tank.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_geometry_water_tank.so");
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
    return vec3<double>(position.x + 10.0*(2.0*Loaded_Funcs.PRNG_source()-1.0), position.y + 10.0*(2.0*Loaded_Funcs.PRNG_source()-1.0), position.z);
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

    return vec3<double>(0.0, 0.0, -1.0);


}


unsigned char geometry_type(const vec3<double> &in){ 

    //This is to ensure that we have not forgot to place our beam pointing downward. Otherwise, the particles will go on travelling until our machine craps out.
    if( (in.x*in.x + in.y*in.y + in.z*in.z) > 1E6) return Material::Black;

    //Space is divided into two parts: Water and Black(hole).
    //
    //Initially I had vacuum above the material, but that resulted in the occasional photon being bounced backward and producing
    // electrons in the vacuum. This is a slight inconvenience, but utlimately an unnecessary one so I fixed it.

    //if( in.z > 0.0) return Material::Vacuum;

    if( (in.z <= 0.0) && (in.z >= -50.0) && (fabs(in.x) <= 15.0) && (fabs(in.y) <= 15.0) ){
        return Material::Water;
    }

    return Material::Black;

}




#ifdef __cplusplus
    }
#endif

