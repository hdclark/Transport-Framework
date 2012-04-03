//Beams.cc - Basic, static beam qualities for a 6MV photon beam. Fluence is given in terms of binned data. 
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
std::string BEAM_TYPE("6MV");

bool VERBOSE = false;


//vec3<double> position(0.0, 0.0, 0.0); //The geometric location of the beam 'spout.'



#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_beam_6MV.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_beam_6MV.so");
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
    const double shifteda = 2.0*ina - 1.0;
    const double shiftedb = 2.0*inb - 1.0;
    const double shiftedc = 2.0*inc - 1.0;
    const double tot = sqrt(shifteda*shifteda + shiftedb*shiftedb + shiftedc*shiftedc);
    return vec3<double>(shifteda/tot, shiftedb/tot, shiftedc/tot);
}
*/


//This function turns a clamped, random, uniformly-distributed real spectrum into an energy spectrum.
//
//It is suitable for determining the energy of photons which have been freshly created at an
// undescribed source.
//
//Units of energy: [E] = MeV.

//double energy_distribution(const double &in){
double energy_distribution(const struct Functions &Loaded_Functions){
    const double in = Loaded_Functions.PRNG_source();

    //A linear regression gives a very nice fit with:
    //
    //  E(x) = g(x) = A*exp(-B*(C-x)**D) - F*exp(-G*(x-H)**2);
    //
    //  where:
    //    A = 6.20411459035791
    //    B = 2.40724692686904
    //    C = 1.0
    //    D = 0.74028089828979
    //    F = 8.8511313684159
    //    G = 6.20064558989353
    //    H = -0.723936500063037
    //
    //    I made a graph of this somewhere...
    //

    double E;

    if((in >= 0.0/100000.0) && (in <= 2480.0/100000.0)){
        E = 0.25; 
    }else if((in > 2480.0/100000.0) && (in <= 15000.0/100000.0)){
        E = 0.5; 
    }else if((in > 15000.0/100000.0) && (in <= 27290.0/100000.0)){
        E = 0.75; 
    }else if((in > 27290.0/100000.0) && (in <= 37590.0/100000.0)){
        E = 1.0;
    }else if((in > 37590.0/100000.0) && (in <= 46310.0/100000.0)){
        E = 1.25; 
    }else if((in > 46310.0/100000.0) && (in <= 53760.0/100000.0)){
        E = 1.5; 
    }else if((in > 53760.0/100000.0) && (in <= 60140.0/100000.0)){
        E = 1.75; 
    }else if((in > 60140.0/100000.0) && (in <= 65680.0/100000.0)){
        E = 2.0;
    }else if((in > 65680.0/100000.0) && (in <= 70460.0/100000.0)){
        E = 2.25; 
    }else if((in > 70460.0/100000.0) && (in <= 74630.0/100000.0)){
        E = 2.5; 
    }else if((in > 74630.0/100000.0) && (in <= 78290.0/100000.0)){
        E = 2.75; 
    }else if((in > 78290.0/100000.0) && (in <= 81510.0/100000.0)){
        E = 3.0;
    }else if((in > 81510.0/100000.0) && (in <= 84330.0/100000.0)){
        E = 3.25; 
    }else if((in > 84330.0/100000.0) && (in <= 86860.0/100000.0)){
        E = 3.5; 
    }else if((in > 86860.0/100000.0) && (in <= 89090.0/100000.0)){
        E = 3.75; 
    }else if((in > 89090.0/100000.0) && (in <= 91060.0/100000.0)){
        E = 4.0;
    }else if((in > 91060.0/100000.0) && (in <= 92790.0/100000.0)){
        E = 4.25; 
    }else if((in > 92790.0/100000.0) && (in <= 94330.0/100000.0)){
        E = 4.5; 
    }else if((in > 94330.0/100000.0) && (in <= 95670.0/100000.0)){
        E = 4.75; 
    }else if((in > 95670.0/100000.0) && (in <= 96840.0/100000.0)){
        E = 5.0;
    }else if((in > 96840.0/100000.0) && (in <= 97850.0/100000.0)){
        E = 5.25; 
    }else if((in > 97850.0/100000.0) && (in <= 98710.0/100000.0)){
        E = 5.5; 
    }else if((in > 98710.0/100000.0) && (in <= 99420.0/100000.0)){
        E = 5.75; 
    }else if((in > 99420.0/100000.0) && (in <= 100000.0/100000.0)){
        E = 6.0;
    }else{
        FUNCERR("Problems with photon energy spectra: unable to assign energy to value " << in);
    }

    return E;
}




#ifdef __cplusplus
    }
#endif

