//Random_MT.cc - Module for generation of pseudo-random numbers using built-in Mersenne-Twistor algorithm.
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

#include <random>
#include <functional>

#include <cmath>

#include "./Misc.h"
#include "./Constants.h"
#include "./Structs.h"

#ifdef __cplusplus
    extern "C" {
#endif

std::string MODULE_NAME(__FILE__);
std::string FILE_TYPE("PRNG");

bool VERBOSE = false;


// <Invisible>
std::mt19937 random_engine;
std::uniform_real_distribution<> random_distribution(0.0, 1.0);
//auto random_src;  //Needed if we want to curry the source call.
// </Invisible>



#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        std::random_device rd; //Generates a hardware-generated random seed.
        random_engine.seed( rd() );
 
        if(VERBOSE) FUNCINFO("Loaded Random_MT.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed Random_MT.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}

bool init_explicit_seed(long int seed){
    //This seeding is not required, but it can be used to re-seed the generator.
    random_engine.seed( seed );
    //random_src = std::bind(random_distribution,random_engine); //No point in currying this..
    return true;
}

double source(void){
    return random_distribution(random_engine); //No point in currying this..
}


//Given three clamped [0,1], random, uniformly-distributed numbers, we return a (three-vector) unit vector pointing in the direction 
// which a randomly-oriented particle will have.
//
//For instance, for an isotropic point source, we just return a random orientation. For a sharply directed beam, we can probably
// just return a constant, directed orientation. In between, we will likely have some angular distribution.
//
//NOTE: It would be better to start with a unit vector and rotate it twice --> no sqrt, only need two randoms.
//
vec3<double> get_random_orientation(void){
    const double shifteda = 2.0*random_distribution(random_engine) - 1.0;
    const double shiftedb = 2.0*random_distribution(random_engine) - 1.0;
    const double shiftedc = 2.0*random_distribution(random_engine) - 1.0;
    const double tot = sqrt(shifteda*shifteda + shiftedb*shiftedb + shiftedc*shiftedc);
    return vec3<double>(shifteda/tot, shiftedb/tot, shiftedc/tot);
}


#ifdef __cplusplus
    }
#endif

