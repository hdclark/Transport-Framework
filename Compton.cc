//Compton.cc - Photon interactions: Compton (incoherent) scattering.
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
std::string INTERACTION_TYPE("COMPTON");

bool VERBOSE = false;

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_compton.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_compton.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}


//Used for the rejection-method scheme. 
inline double normalized_maximum_value(const double &alpha){
    if( alpha <= 0.01 ) return 7.0;
    if( alpha > 10000.0 ){
        FUNCERR("Rejection scheme is not properly tuned to handle photons with energies larger than 10000. Expect slow results if proceeding.");
        //return 0.061;
    }
    return 0.0125 + 0.04 + 1.0/(0.1129064864059787*pow(alpha,0.578759550187321)+0.1173113799014099*pow(alpha,-0.0235034578761817));
}

//This function returns a normalized first-order Klein-Nishina differential cross section $\frac{2}{r_{e}^{2}} \frac{d\sigma}{d\theta}.$
// It is used to randomly assign a photon scattering angle (theta) when the incoming photon is of some energy. 
// It is used in a rejection-method scheme for a stochastic numerical inversion of the probability density function $\frac{d\sigma}{d\theta}.$
// It should not be used anywhere else, lest the normalization is taken into account!
inline double normalized_differential_cross_section(const double &a, double t){  //alpha and theta.
    return  6.283185307179586*sin(t)*pow(a*(1.0-cos(t))+1.0,-2.0)*(pow(cos(t),2.0)+1.0)*(pow(a,2.0)*pow(a*(1.0-cos(t))+1.0,-1.0)*pow(1.0-cos(t),2.0)*pow(pow(cos(t),2.0)+1.0,-1.0)+1.0);
}

//This function returns a random theta for a given photon energy which conforms to the Klein-Nishina angular distribution.
// The angle theta represents the (planar) photon scattering angle.
double KN_angular_distribution(const double &Ephot, const struct Functions &Loaded_Functions){
    const double alpha = Ephot / electron_mass;

    double theta;
    const double ymax = normalized_maximum_value(alpha); //The smaller this value is, the faster we will get a suitable theta. Too low is bad though!
    double y;

    do{
        theta = Loaded_Functions.PRNG_source()*M_PI;  //Within [0,pi].
        y     = Loaded_Functions.PRNG_source()*ymax;  //Within [0,m].

        //This is for testing. Turn off when done.
        if( ymax <= normalized_differential_cross_section( alpha, theta )) FUNCWARN("Rejection fmax should be increased. Discrep. is " << normalized_differential_cross_section( alpha, theta ) - ymax);


    }while( y > normalized_differential_cross_section( alpha, theta ));

    return theta;
} 



void scatter(std::unique_ptr<base_particle> A, const struct Functions &Loaded_Functions){
    //Implements a Compton scattering event. Assumes ownership of the particle, so sink it back into memory when finished.

    //Needed in this function: 
    // - A uniformly random, clamped number generator.
    // - Access to the memory sink function.

    if(A->get_type() != Particletype::Photon){
        FUNCERR("Compton scattering only implemented for photons. Attempted to perform scatter event on particle of type " << A->get_type());
    }

//    if(PHOTON_SEPUKU_DONATE_TO_ELECTRON) FUNCERR("Photon sepuku - electron energy donation - is not supported in " << __FILE__ );
    if(PHOTON_SEPUKU_DISAPPEAR)          FUNCERR("Photon sepuku - photon energy disappearance - is not supported in " << __FILE__ );
    if(PHOTON_SEPUKU_DISTRIBUTE)         FUNCERR("Photon sepuku - spatial energy distribution - is not supported in " << __FILE__ );


    const double incoming_photon_E = A->get_energy();

    //Photon scattering angle is chosen according to the Klein-Nishina equation. Electron angle follows. Planar nature means random plane orientation.
    const double theta  = KN_angular_distribution(incoming_photon_E, Loaded_Functions);      //This defines the photon scattering angle (from incoming photon, in plane.)
//    const double phi    = atan2( -sin(theta) / (cos(theta) - incoming_photon_E/photon_E) );  //This defines the electron scattering angle (from incoming photon, in plane.)

    const double R      = Loaded_Functions.PRNG_source()*2.0*M_PI;    //This defines the plane orientation. It is random.


    //Log the photon angular distribution as a function of photon energy.
    if(LoggingQuantities::PhotonAngularSampled){
        Loaded_Functions.generic_logging("Photon_Angular_Distribution") << incoming_photon_E << " " << theta << " " << R << std::endl;
    }

    //Determine how much energy the outgoing photon will have.
    const double photon_E     = incoming_photon_E/(  1.0 + (incoming_photon_E/electron_mass)*(1.0-cos(theta)));
    const bool   photon_alive = (photon_E > PHOTON_SEPUKU_ENERGY_THRESHOLD) ? true : false;

    const double phi    = atan2( -sin(theta) , (cos(theta) - incoming_photon_E/photon_E) );  //This defines the electron scattering angle (from incoming photon, in plane.)

    //If the outgoing photon is of 'sufficiently small' energy, and not likely to contribute too much (compared with the higher-energy 
    // photons, we will simply 'donate' its energy to the electron.

    // This is the PHOTON_SEPUKU_DONATE_TO_ELECTRON routine.
    const double electron_E = (photon_alive) ? (electron_mass + incoming_photon_E - photon_E) : (electron_mass + incoming_photon_E);


    //Create an electron at the position of the photon.                                                             
    const double B_mom_mag  = sqrt( electron_E*electron_E - electron_mass*electron_mass );



    vec3<double> B_momentum = rotate_unit_vector_in_plane((A->get_relativistic_three_momentum3()).unit(), -phi, R) * B_mom_mag;  //Note: this phi should be negative due to coordinate system and defntn.
////    vec3<double> B_momentum = A->get_relativistic_three_momentum3();


    std::unique_ptr<base_particle> B = Loaded_Functions.electron_factory( electron_E, A->get_position3(), B_momentum );
    B->Interactions.push_back( an_interaction(Interactiontype::Creation, Material::Unknown, B->get_energy(), B->get_position3()));

    //Log the fraction of kinetic energy transferred to recoil electrons as a function of incoming photon energy.
    if(LoggingQuantities::FractionTransferredCompton){
        Loaded_Functions.generic_logging("Fraction_Transferred_Compton") << incoming_photon_E << " " <<  ((electron_E-electron_mass)/incoming_photon_E) << std::endl;
    }

    //Push the electron back into memory.
    Loaded_Functions.particle_sink( std::move( B ) );

    if(photon_alive){

        //----------
        //Why am I creating a second photon? Is it to deal with momentum issues? Wouldn't it be better to simply set_ the E,pos,mom ?

        
        //Create the second photon.
        //vec3<double> C_momentum = (A->get_relativistic_three_momentum3()).unit() * photon_E;    
        //C_momentum = Loaded_Functions.get_random_orientation();


        vec3<double> C_momentum = rotate_unit_vector_in_plane((A->get_relativistic_three_momentum3()).unit(), theta, R) * photon_E;
////        vec3<double> C_momentum = (A->get_relativistic_three_momentum3()).unit() * photon_E;


        std::unique_ptr<base_particle> C = Loaded_Functions.photon_factory( photon_E, A->get_position3(), C_momentum );
        C->Interactions = A->Interactions;

        //Push the electron back into memory, and let the photon be destroyed.
        Loaded_Functions.particle_sink( std::move( C ) );

        //DO NOT send A to the graveyard. We are basically *duplicating* the original photon here...
        //----------

    }else{
        //Let the photon be destroyed.
        Loaded_Functions.particle_graveyard( std::move( A ) );
    }


    return;
}









#ifdef __cplusplus
    }
#endif

