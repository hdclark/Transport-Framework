//Pair.cc - Photon interactions: Pair-production "scattering."
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
std::string INTERACTION_TYPE("PAIR");

bool VERBOSE = false;

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_pair.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_pair.so");
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
    //Implements a Pair-production scattering event. Assumes ownership of the particle, so sink it back into memory when finished.

    //Needed in this function: 
    // - A uniformly randomly oriented number generator.
    // - Access to the memory sink function.
    // - Access to the electron class constructor.
    // - Access to the positron class constructor.

    if(A->get_type() != Particletype::Photon){
        FUNCERR("Pair production only implemented for photons. Attempted to perform scatter event on particle of type " << A->get_type());
    }


    if(A->get_energy() < 2.0*electron_mass){
        FUNCWARN("  \\/--This particle has E, position, momentum, and type: " << A->get_energy() << " " << A->get_position3() << " " << A->get_relativistic_three_momentum3() << " " << (int)(A->get_type()) );
        FUNCERR("Attempted pair production without enough energy (" << A->get_energy() << "). Are the cross sections accurate?")
    }

    const double Ephoton = A->get_energy();

    //These are sampled from a Gaussian distribution about a (rough) mean value. Technically only valid at HIGH photon energy ( hn >> 2*m*c*c ),
    // but should be suspect at all energies!
    //
    //Note that the deflection angles are sampled separately. We assume there is no correlation using this method!
    //
    //Note that we have to avoid pathological angles and kinematically-impossible cases explicitly, due to our approximate treatment.
    double theta_elec; 
    double theta_posi;  ///FIXME - Gaussian distribution!
    //Angles individually cannot be:
    //   -->  -pi
    //   -->  0
    //   -->  pi
    // and the separation of the angles also cannot be these values. If we get these values, re-sample!
 
    do{
        //This is a standard Gaussian with mean 0 and variance 1.0.
        theta_elec = sqrt(2.0*log(1.0/Loaded_Functions.PRNG_source()))*cos(2.0*M_PI*Loaded_Functions.PRNG_source());  //Box-Muller. Use sin or cos!
        theta_posi = sqrt(2.0*log(1.0/Loaded_Functions.PRNG_source()))*sin(2.0*M_PI*Loaded_Functions.PRNG_source());

        //This shifts the mean and scales with the variance.
        theta_elec = (theta_elec*0.5 + 1.0)/(-1.0+0.5*Ephoton/electron_mass);
        theta_posi = (theta_posi*0.5 + 1.0)/(-1.0+0.5*Ephoton/electron_mass);

        //This spins the positron angle around so that it is a positive value describing a negative angle. See diagram (positron angle is defined opposite orientation to electron angle.)
        theta_posi *= -1.0;

    }while(  (fmod(fabs(theta_elec),M_PI) < 1E-11) 
             || (fmod(fabs(theta_posi),M_PI) < 1E-11)
             || (fmod(fabs(theta_posi-theta_elec), M_PI) < 1E-11) );  // 1E-11 is chosen basically for no good reason. It should be large, but small enough to avoid numerical instability.
 
    //This is the plane orientation angle. It specifies a plane about the photon's momentum vector. It 'eats' up a degree of freedom.
    const double R = Loaded_Functions.PRNG_source()*2.0*M_PI;

    //Knowing the angles and the photon energy, the momentum, energy can be found.    
    const double elec_mom_mag  =  Ephoton*sin(theta_posi)/sin(theta_posi-theta_elec);
    const double elec_E        =  sqrt( elec_mom_mag*elec_mom_mag + electron_mass*electron_mass );
    vec3<double> elec_momentum =  rotate_unit_vector_in_plane((A->get_relativistic_three_momentum3()).unit(), theta_elec, R) * elec_mom_mag; 
  
    const double posi_mom_mag  = -Ephoton*sin(theta_elec)/sin(theta_posi-theta_elec);
    const double posi_E        =  sqrt( posi_mom_mag*posi_mom_mag + electron_mass*electron_mass );
    vec3<double> posi_momentum =  rotate_unit_vector_in_plane((A->get_relativistic_three_momentum3()).unit(), theta_posi, R) * posi_mom_mag;  //Note the negative sign is already applied to the angle earlier.


    //Push an electron into memory.
    {
        std::unique_ptr<base_particle> temp = Loaded_Functions.electron_factory(elec_E,A->get_position3(),elec_momentum); 
        temp->Interactions.push_back( an_interaction(Interactiontype::Creation, Material::Unknown, temp->get_energy(), temp->get_position3()));
        Loaded_Functions.particle_sink( std::move( temp ) );
    }

    //Push a positron into memory.
    {
        std::unique_ptr<base_particle> temp = Loaded_Functions.positron_factory(posi_E,A->get_position3(),posi_momentum);
        temp->Interactions.push_back( an_interaction(Interactiontype::Creation, Material::Unknown, temp->get_energy(), temp->get_position3()));
        Loaded_Functions.particle_sink( std::move( temp ) );
    }

    //Photon bites the dust!    
    Loaded_Functions.particle_graveyard( std::move( A ) );

    return;
}









#ifdef __cplusplus
    }
#endif

