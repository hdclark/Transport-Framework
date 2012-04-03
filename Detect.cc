//Detect.cc - A routine for particles to enter when they are within the geometry of a detector.
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

#include <memory>
#include <cmath>

#include "./Misc.h"

#include "./Constants.h"
#include "./Structs.h"

#ifdef __cplusplus
    extern "C" {
#endif

std::string MODULE_NAME(__FILE__);
std::string FILE_TYPE("INTERACTION");
std::string INTERACTION_TYPE("DETECTION");

bool VERBOSE = false;

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_detect.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_detect.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}




void scatter(std::unique_ptr<base_particle> A, const struct Functions &Loaded_Functions){
    //Implements a simple energy dump event. Destroys particle afterward by expiring it.
    //
    //It is called 'scatter' to maintain logical consistency for function naming within the interaction files.

    //Needed in this function: 
    // - Access to logging routines.
    

    //For detector geometry.
    //Loaded_Functions.generic_logging("Detector") << A->Interactions[0].energy << " " << A->get_position3() << " " << (A->Interactions.size() - 1) << std::endl;
    Loaded_Functions.generic_logging("Detector") << A->Interactions[0].energy << " " << (A->get_position3()).x << " " << (A->get_position3()).y << " " << (A->get_position3()).z << " " <<   (A->Interactions.size() - 1) << std::endl; 


    return;
}









#ifdef __cplusplus
    }
#endif

