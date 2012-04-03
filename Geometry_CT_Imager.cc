//Geometry_CT_Imager.cc - Simulates a cylindrical CT imager centered about (0,0,0) and lying in the x-z plane. Detectors are at the bottom and are collimated with comb filters.
//
//The object to be imaged is separated from the imager. The object to be imaged is considered to move with time - NOT the imager!
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

double SMALLEST_FEATURE = 0.5;     //The smallest feature in the expected path of the beam through the geometry - useful for transporting particles through a vacuum in a sensible way. 
                                   //Note that this is a 'statistically-relevant' feature. It can be slightly higher than the geometrically smallest feature.

bool VERBOSE = false;

const int NUMB_OF_CELLS = 3;      //There is a bug in this code that somehow doubles this number!

const double r_out         = 15.0;
const double r_source      = r_out - 1.0;
const double r_coll        = r_out - 2.0;
const double r_det         = r_out - 1.0;
const double r_clearance   = 13.0;   // = min(r_out,r_source,r_coll,r_det);

const double dtheta_source  = 1.0/r_source;  //s = r * theta
const double dtheta_det     = 7.5/r_det;
const double dtheta_cell    = dtheta_det/((double)(NUMB_OF_CELLS));

//const double dtheta_coll    = dtheta_cell/10.0;                              // <------------------ Keep comb collimator. 
const double dtheta_coll    = 0.0;                                           // <------------------ Remove comb collimator.

const double thickness     = 0.5;

//----

const double theta_det_min = (3.0/4.0)*2.0*M_PI - 0.5*dtheta_det;
const double theta_det_max = (3.0/4.0)*2.0*M_PI + 0.5*dtheta_det;

vec3<double> position(0.0, 0.0, -14.0); //The geometric location of the beam point 'spout.'



#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_geometry_CT_imager.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_geometry_CT_imager.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}

void set_position(const vec3<double> &in){ //External source position setter.
    position = in;
    return;
}

vec3<double> get_position(const struct Functions &Loaded_Funcs){ //External source position getter.
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



unsigned char geometry_type(const vec3<double> &in){ 

    //Please consult the notes on this geometry file. Documentation here will be scant and likely not useful! (...irony...)

    const double r = sqrt(in.x*in.x + in.y*in.y + in.z*in.z);
    const double x = in.x;
    const double y = in.y;
    const double z = in.z;

    //Easy stuff: killing off particles which are outside the detector geometry.
    if( fabs(y) > 0.5*thickness ) return Material::Black;
    if(  r > r_out ) return Material::Black;

/*
    if((x==0.0) && (z==0.0)){ 
        FUNCWARN("Particle was detected at precisely (0,0,z). Removing particle for convenience.");
        return Material::Black; 
    }
*/

    const double theta = atan2(z,x) + M_PI;  // In the range [0:2*pi].
    const double s = r * theta; //The effective distance the particle is 'wrapped around' the origin.
    //------


    //If the particle is outside the clearance distance, we do not have to worry about checking the sample geometry..
    if(r > r_clearance){

        //The source is considered to be vacuum, and we shouldn't (often?) expect to have particles hitting the source anyways..


        //These comb collimators are perfect black holes. Replace with a more realistic material when it is implemented.
        if((r < r_det) && (r >= r_coll)){
            //Get the cell thickness at this distance from the origin.
            const double ds_each_cell  = dtheta_cell * r / NUMB_OF_CELLS;

            //Get the comb thickness at this distance from the origin.
            const double ds_each_comb  = dtheta_coll * r;

            //Get the absolute distance the collimator angle is 'wrapped around' the origin. This is not supposed to make physical sense!
            const double s_coll_min     = theta_det_min * r;
            const double s_coll_max     = theta_det_max * r;

            //If the particle's s is outside of this range, it is not within the collimator bounding box.
            if((s<s_coll_min) || (s>s_coll_max)){
                return Material::Vacuum;
            }

            //Otherwise, find the distance it is wound into the collimator and modulo it with the collimator ds.
            const double s_into_collimator   =  s - s_coll_min;

            //If the arc length % the individual cell length is shorter than the individual comb length, it is in the comb!
            if( fmod(s_into_collimator, ds_each_cell) <= ds_each_comb ){
                return Material::Black;
            }

            //Otherwise, it is in the interstitial space between combs.
            return Material::Vacuum;

        }


        //The exact detector cell array is not terribly important here. All we have to know is that we are in the detector - we can determine
        // exactly which one with some post-processing (or additional coding at a later date...)
        if((r >= r_det) && (r <= r_out)){ // <--- Redundant?

            //Get the absolute distance the collimator angle is 'wrapped around' the origin. This is not supposed to make physical sense!
            const double s_coll_min     = theta_det_min * r;
            const double s_coll_max     = theta_det_max * r;

            if((s > s_coll_min) && (s < s_coll_max)){
                return Material::Detector;
            }

            //Otherwise, we are not in the detector. In fact, we are likely NOT going to hit the detector, either. But I will let the code
            // decide this (maybe we will lose some backscatter if we cull prematurely?)
            return Material::Vacuum;
        }

    //Object geometry here...
    }else{

        //FIXME - put some interesting geometry here, load in some data, or do something other than 'stinking water.'
//        if(r < 0.07*r_clearance) return Material::Water;



        return Material::Vacuum;


    }



    //If we are in no-man's-land, we cull and warn.
    FUNCWARN("Found a particle outside anticipated geometry (at " << in << "). Culling particle and continuing.");
    return Material::Black;

}


#ifdef __cplusplus
    }
#endif

