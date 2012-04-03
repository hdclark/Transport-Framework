//SlowDown.cc - Handles CSDA (Continuously slowing-down approximation) for charged particles. NOTE: Assumes constant stopping power.
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
std::string INTERACTION_TYPE("SLOWDOWN");

bool VERBOSE = false;

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_slowdown.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_slowdown.so");
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
    //Implements a continuous energy dump event. Destroys particle afterward by expiring it.
    //
    //It is called 'scatter' to maintain logical consistency for function naming within the interaction files.

    //Needed in this function: 
    // - Access to accumulated media energy routines.
    // - Access to logging routines.
    // - Access to voxel-geometry routines.
    
    if((A->get_type() != Particletype::Electron) && (A->get_type() != Particletype::Positron)){
        FUNCWARN("Attempted to perform scatter_slowdown (CSDA) on an uncharged particle type. Ignoring particle!"); 
        return;
    }

  
    //The current energy of the particle will be the INITIAL energy of the particle PRIOR to slowdown.
    const double initial_E = A->get_energy();

    //The final energy will be rest mass. Maybe it should be a threshold instead? (probably doesn't matter much!)
    const double final_E = electron_mass;

    //The current position of the particle will be the FINAL position of the particle, AFTER slowdown
    vec3<double> final_pos = A->get_position3();

    //The initial position (PRIOR to slowdown) can be found in the interaction history.
//    vec3<double> initial_pos = A->Interactions[A->Interactions.size()-1].position;
    if((A->Interactions.size() != 2) || (A->Interactions[0].interaction != Interactiontype::Creation)){
        FUNCERR("This electron has undergone more than one interaction. CSDA mixing with other interactions is not supported! (This may be the result of having vacuum in the geometry)");
    }

    vec3<double> initial_pos = A->Interactions[0].position;

    //We now have enough info from the particle to raycast it through the voxel geometry if we assume the stopping power is constant. 
    // In reality, it would be much more cumbersome to handle this way  FIXME
    
    // -----> call the voxel-recording routine here!
    Loaded_Functions.voxel_accumulation(initial_E, initial_pos,  final_E, final_pos, Loaded_Functions);

    return;
}


#ifdef __cplusplus
    }
#endif

