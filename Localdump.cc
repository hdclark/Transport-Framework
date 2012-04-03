//Localdump.cc - Generic particle interactions: dump all particle (kinetic) energy locally and destroy particle.
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
std::string INTERACTION_TYPE("LOCALDUMP");

bool VERBOSE = false;

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_localdump.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_localdump.so");
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
    // - Access to accumulated media energy routines.
    // - Access to logging routines.
    


    if(A->get_type() == Particletype::Photon){
        //Do something with the photon energy here.


    }else{
        //Do something with the particles energy here.

        if(LoggingQuantities::ElectronStoppingPos){
            vec3<double> pos = A->get_position3();
            Loaded_Functions.generic_logging("Electron_Stopped")  << pos.x << " " << pos.y << " " << pos.z << " " << sqrt(pos.x*pos.x+pos.y*pos.y+pos.z*pos.z) << " " << A->get_energy() << std::endl;
        }


        if(USE_CSDA){ 
            Loaded_Functions.voxel_localdump( (A->get_energy() - A->get_mass()), A->get_position3() , Loaded_Functions);
        }

    }

//    Loaded_Functions.particle_graveyard( std::move( A ) );
    return;
}









#ifdef __cplusplus
    }
#endif

