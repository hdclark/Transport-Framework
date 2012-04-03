//Photoelectric.cc - Photon interactions: photoelectric effect.
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
std::string INTERACTION_TYPE("PHOTOELECTRIC");

bool VERBOSE = false;

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_photoelectric.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_photoelectric.so");
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
    //Implements a photoelectric effect event. Assumes ownership of the particle, so sink it back into memory when finished.
    //
    //It is called 'scatter' to maintain logical consistency for function naming within the interaction files.

    //Needed in this function: 
    // - Access to the electron class constructor.
    // - Access to the memory sink function.

    if(A->get_type() != Particletype::Photon){
        FUNCERR("Photoelectric effect only implemented for photons. Attempted to perform scatter event on particle of type " << A->get_type());
    }


    const double electron_energy = electron_mass + A->get_energy() - water_binding_energy_oxygen_K;

    if(electron_energy < electron_mass ){
        FUNCERR("Attempted photoelectric effect without enough energy (" << A->get_energy() << " and binding energy is " \
          << water_binding_energy_oxygen_K << "). Are the cross sections accurate?" << std::endl << \
          " This particle has E, position, momentum, and type: " << A->get_energy() << " " << A->get_position3() << \
          " " << A->get_relativistic_three_momentum3() << " " << (int)(A->get_type()));
    }


    //Simplified photoelectric effect. From the assignment:
    //
    //  "For photoelectric effect: Electron_Energy (kinetic) = Photon_Energy – (543.1 eV); i.e.
    //  incident photon energy minus the binding energy of the K-shell of the Oxygen
    //  atom in the water molecule. Ignore any characteristic photons or Auger
    //  electrons produced in this interaction."


    //Create an electron with energy from the photon and at the position of the photon.
    const double B_mom_mag = sqrt( electron_energy*electron_energy - electron_mass*electron_mass );
    std::unique_ptr<base_particle> B = Loaded_Functions.electron_factory( electron_energy, A->get_position3(), (A->get_relativistic_three_momentum3()).unit() * B_mom_mag );

    B->Interactions.push_back( an_interaction(Interactiontype::Creation, Material::Unknown, B->get_energy(), B->get_position3()));

    //Push the electron back into memory, and let the photon be destroyed.
    Loaded_Functions.particle_sink( std::move( B ) );

    Loaded_Functions.particle_graveyard( std::move( A ) );

    return;
}









#ifdef __cplusplus
    }
#endif

