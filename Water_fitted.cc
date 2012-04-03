//Water_fitted.cc - Non-interactive module for handling water mediums. Uses fitted lookup tables for mass attenutation coefficients.
//
//Programming notes:
//  -Do not make items here "const", because they will not show up when loading.
//  -Avoid using macro variables here because they will be obliterated during loading.
//  -Wrap dynamically-loaded code with extern "C", otherwise C++ compilation will mangle function names, etc.
//
// From man page for dlsym/dlopen:  For running some 'initialization' code prior to finishing loading:
// "Instead,  libraries  should  export  routines using the __attribute__((constructor)) and __attribute__((destructor)) function attributes.  See the gcc 50.0o pages for
//       50.0ormation on these.  Constructor routines are executed before dlopen() returns, and destructor routines are executed before dlclose() returns."
//   ---for instance, we can use this to seed a random number generator with a random seed. However, in order to pass in a specific seed (and pass that seed to the library)
//      we need to define an explicitly callable initialization function. In general, these libraries should have both so that we can quickly adjust behaviour if desired.
//  

#include <iostream>
#include <string>
#include <vector>

#include <cmath>

#include "./Misc.h" //Using isininc macro from here.

#include "./Constants.h"
#include "./Structs.h"
 
#ifdef __cplusplus
    extern "C" {
#endif

std::string MODULE_NAME(__FILE__);
std::string FILE_TYPE("MEDIUM");
std::string MEDIUM_TYPE("WATER");

bool VERBOSE = false;




double photon_mass_coefficient_coherent(const double &E){
    if(!isininc(0.0, E, 50.0)) FUNCERR("Water-Photon-coherent mass coefficient is outside of range of data (0-50 MeV) at " << E );
    return -(5.263157894736842E-5)*(exp(-243.457*E)-1.0)*pow(exp(-243.457*E)+1.0,-3.44416)*pow(E,-2.0); 
}

double photon_mass_coefficient_compton(const double &E){
    if(!isininc(0.0, E, 50.0)) FUNCERR("Water-Photon-compton mass coefficient is outside of range of data (0-50 MeV) at " << E );
    return 33.4605*pow(53.36492536915188*pow(E,1.1555)+392.7205322148653*pow(E,0.46798)+29.93707227407988*pow(E,-0.357379)+(6.248672157166602E-4)*pow(E,-2.18652),-1.0);
}

double photon_mass_coefficient_photoelectric(const double &E){
    if(!isininc(0.0, E, 50.0)) FUNCERR("Water-Photon-photoelectric mass coefficient is outside of range of data (0-50 MeV) at " << E );
    if( E <= water_binding_energy_oxygen_K ) return 0.0;

    const double A=1.1;
    const double C = 550000.0;
    const double B = 3.2;
    const double D = 1200.0;
    return (( pow(E,-A) + pow(E,-B))/C) * (1.0-exp(-E*D))/(1.0+exp(-E*D));
//    return pow(550000.0*exp(1200.0*E)+550000.0,-1.0)*(exp(1200.0*E)*(pow(E,-1.1)+pow(E,-3.2))-pow(E,-1.1)-pow(E,-3.2));
}

double photon_mass_coefficient_pair_triplet(const double &E){
    if(!isininc(0.0, E, 50.0)) FUNCERR("Water-Photon-Pair production mass coefficient is outside of range of data (0-50 MeV) at " << E );
    if( E < 1.2 ) return 0.0;
    if( E < 2.0*electron_mass ) return 0.0;
    return (2.60355E-5)*(E-2.0*electron_mass)+0.01*(1.0-1.0*exp(-0.142367*(E-2.0*electron_mass)))*pow(exp(-0.142367*(E-2.0*electron_mass))+1.0,-1.94953)+(8.0E-4)*exp(-0.07*pow(E-7.522,2.0));
}


#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_water_fitted.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_water_fitted.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}


double mean_free_path( base_particle *in, const double &clamped ){
    //Check if we are dealing with photons.
    if(in->get_type() == Particletype::Photon){ 
        //Grab the energy of the particle.
        const double E = in->get_energy();

        //Determine the total interaction cross-section at this energy. (Probably should not trust tabulated data here, particularly if it
        // has been interpolated!)
        const double s_coherent       = photon_mass_coefficient_coherent(E);
        const double s_compton        = photon_mass_coefficient_compton(E);
        const double s_photoelectric  = photon_mass_coefficient_photoelectric(E);
        const double s_pair_triplet   = photon_mass_coefficient_pair_triplet(E);
        const double s_tot            = s_coherent + s_compton + s_photoelectric + s_pair_triplet;
        const double mu_tot           = s_tot*water_mass_density;

        return -log(clamped)/mu_tot;
    }else if(in->get_type() == Particletype::Electron){
        //We consider electrons to move zero distance before interacting.
        return 0.0;

    }else if(in->get_type() == Particletype::Positron){
        //We consider positrons to move zero distance before interacting.
        return 0.0;
    }

    FUNCERR("Mean-free-path for unaccounted-for particle requested");
    return 0.0;
}

unsigned char which_interaction( base_particle *in, const double &clamped ){
    //Check if we are dealing with photons.
    if(in->get_type() == Particletype::Photon){
        //Grab the energy of the particle.
        const double E = in->get_energy();
    
        //Determine the total interaction cross-section at this energy. (Probably should not trust tabulated data here, particularly if it
        // has been interpolated!)
        const double s_coherent       = photon_mass_coefficient_coherent(E);
        const double s_compton        = photon_mass_coefficient_compton(E);
        const double s_photoelectric  = photon_mass_coefficient_photoelectric(E);
        const double s_pair_triplet   = photon_mass_coefficient_pair_triplet(E);
        const double s_tot            = s_coherent + s_compton + s_photoelectric + s_pair_triplet;
    
        if( isininc(0.0, clamped*s_tot, s_coherent) ){
            return Interactiontype::Coherent;
        }else if( isininc(s_coherent, clamped*s_tot, s_coherent+s_compton) ){
            return Interactiontype::Compton;
        }else if( isininc(s_coherent+s_compton, clamped*s_tot, s_coherent+s_compton+s_photoelectric) ){
            return Interactiontype::Photoelectric;
        }else{ //}else if( isininc(s_coherent+s_compton+s_photoelectric, clamped*s_tot, s_coherent+s_compton+s_photoelectric+s_pair_triplet) ){
            return Interactiontype::Pair;
        }

    }else if(in->get_type() == Particletype::Electron){
        //We consider electrons to *always* interact with a total dump of energy (locally.)
        return Interactiontype::LocalDump;

    }else if(in->get_type() == Particletype::Positron){
        //We consider positrons to *always* interact with a total dump of energy (locally.)
        return Interactiontype::LocalDump;
    }

    FUNCERR("Interaction choice for unaccounted-for particle requested");
    return 0.0;
}


//This is used for slow media to avoid having to compute the mass attenuation coefficients twice.
void mean_free_path_and_which_interaction( base_particle *in, const double &clamped1, const double &clamped2, unsigned char &which, double &mfp){
    //Check if we are dealing with photons.
    if(in->get_type() == Particletype::Photon){
        //Grab the energy of the particle.
        const double E = in->get_energy();

        //Determine the total interaction cross-section at this energy. (Probably should not trust tabulated data here, particularly if it
        // has been interpolated!)

        const double s_coherent       = photon_mass_coefficient_coherent(E);
        const double s_compton        = photon_mass_coefficient_compton(E);
        const double s_photoelectric  = photon_mass_coefficient_photoelectric(E);
        const double s_pair_triplet   = photon_mass_coefficient_pair_triplet(E);
        const double s_tot            = s_coherent + s_compton + s_photoelectric + s_pair_triplet;
        const double mu_tot           = s_tot*water_mass_density;

        mfp = -log(clamped2)/mu_tot;

        if( isininc(0.0, clamped1*s_tot, s_coherent) ){
            which = Interactiontype::Coherent;
        }else if( isininc(s_coherent, clamped1*s_tot, s_coherent+s_compton) ){
            which = Interactiontype::Compton;
        }else if( isininc(s_coherent+s_compton, clamped1*s_tot, s_coherent+s_compton+s_photoelectric) ){
            which = Interactiontype::Photoelectric;
        }else{ //}else if( isininc(s_coherent+s_compton+s_photoelectric, clamped1*s_tot, s_coherent+s_compton+s_photoelectric+s_pair_triplet) ){
            which = Interactiontype::Pair;
        }

        return;

    }else if(in->get_type() == Particletype::Electron){
        //We consider electrons to *always* interact with a total dump of energy (locally.)
        mfp = 0.0;
        which = Interactiontype::LocalDump;

        return;
    }else if(in->get_type() == Particletype::Positron){
        //We consider positrons to *always* interact with a total dump of energy (locally.)
        mfp = 0.0;
        which = Interactiontype::LocalDump;

        return;
    }

    FUNCERR("Interaction choice for unaccounted-for particle requested");

    return;
}



#ifdef __cplusplus
    }

#endif

