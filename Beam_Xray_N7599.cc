//Beam_Xray_N7599.cc - Defines an approximate spectra for a Hamamatsu N7599 xray source.
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
std::string FILE_TYPE("BEAM");
std::string BEAM_TYPE("XRAY");

bool VERBOSE = false;


//vec3<double> position(0.0, 0.0, -14.0); //The geometric location of the beam 'spout.'



#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_beam_xray_N7599.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_beam_xray_N7599.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}

/*
void set_position(const vec3<double> &in){
    position = in;
    return;
} 

vec3<double> get_position(void){
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

    const double dtheta = 7.5/28.0;  //Approximate apparent width of the detector from the point of the source.
    const double theta  = (ina-0.5)*dtheta;

    const double dphi   = 0.5/28.0; //Approximate apparent thickness of the CT setup at the far edge.
    const double phi    = (inb-0.5)*dphi; 
    return vec3<double>(sin(theta)*cos(phi), sin(phi), cos(theta)*cos(phi));

//    return vec3<double>(0.0,0.0,1.0);
}
*/



//This function returns a normalized (to f(8.4000000) = 99.9763803675478...) Hamamatsu N7599 emission spectrum. 
// It is used in a rejection-method scheme for a stochastic numerical inversion of the probability density function.
// Do NOT use anywhere where absolute output is required - this is total-output agnostic!
inline double normalized_spectral_intensity(const double &E){
    if(E < 2.0000E-3) return 0.0;
    if(E > 15.714E-3) return 0.0;

    return 1.7*(1.0-0.9*pow(2.718281828459045,-0.6155722066724582*pow(1000.0*E-2.0,0.35)))*pow(1.0-0.8*pow(2.718281828459045,-0.75*(1000.0*E-1.7)),3.0)*(55.0-3500.0*E)+69.0*pow(2.718281828459045,-28.0*pow(1000.0*E-8.4,2.0))+20.0*pow(2.718281828459045,-28.0*pow(1000.0*E-9.67,2.0));
}


//This function returns a random energy for a given clamped random which conforms to the Hamamatsu N7599 emission spectrum.
//
//It is suitable for determining the energy of photons which have been freshly created at an
// undescribed source.
//
//Units of energy: [E] = MeV.
double energy_distribution(const struct Functions &Loaded_Functions){

    const double energy_min = 2.0000E-3;
    const double energy_max = 15.714E-3;
    double energy;

    const double ymin = 0.0;
    const double ymax = 100.0; //Doesn't have to be exactly max, but it DOES have to be slightly higher than max.
    double y;

    do{
        energy = Loaded_Functions.PRNG_source()*(energy_max - energy_min) + energy_min;  //Within [energy_min,energy_max].
        y      = Loaded_Functions.PRNG_source()*(ymax-ymin) + ymin;  //Within [0,ymin].

    }while( y > normalized_spectral_intensity( energy ));


    //Adjusting for a useable image quality..

//return 7.5*energy;
return 2.5*energy;

    //Looks OK. A little higher would probably be sweet-spot, but it is a little too finicky..
    return 10.0 * energy;

    //This essentially makes the water transparent! No noticeable perturbation during testing..
    return 25.0 * energy; 

    //Appears to INCREASE signal for water object ??
    return 100.0 * energy;   //FIXME ! Note: we bump this up to MeV range because it is damned impossible to see anything interesting otherwise...


    //ACTUAL:
    return energy;
}


#ifdef __cplusplus
    }
#endif

