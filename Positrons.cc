//Positrons.cc - Basic, non-interactive module for handling positrons.
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
std::string FILE_TYPE("PARTICLE");
std::string PARTICLE_TYPE("POSITRON");

bool VERBOSE = false;

//const double positron_mass   = 0.510998910;
//const double positron_charge = 1.0;

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_positrons.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_positrons.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}


//Overloaded base particle class. Relies on polymorphism with base_particle to be used for dynamic linking!
class Positron : public base_particle {
    public:
        //Constructors.
        // Signature of base_particle mega-constructor:
        //  const unsigned char &type_in, const double &mass_in, const double &charge_in, const double &energy_in, const vec4<double> &X_in, const vec4<double> &U_in
        Positron() : base_particle(Particletype::Positron, positron_mass, positron_charge, 0 ) { } 
        Positron(const vec4<double> &X_in, const vec4<double> &U_in) : base_particle(Particletype::Positron, positron_mass, positron_charge, 0, X_in, U_in) { }
        Positron(const double &energy_in, const vec4<double> &X_in, const vec4<double> &U_in) : base_particle(Particletype::Positron, positron_mass, positron_charge, energy_in, X_in, U_in) { }

        //Overloaded methods.
        virtual double get_energy(void) const { return energy; }
        virtual void set_energy(const double &ein){ energy = ein; }
        virtual double get_speed(void) const { return sqrt(1.0 - pow(mass/energy,2.0)); }
};


//This is a factory function which lets us create objects on the heap when we dynamically load.
std::unique_ptr<base_particle> particle_factory(const double &energy_in, const vec3<double> &position, const vec3<double> &orientation){
//    vec4<double> position4(0.0, position);
//    vec4<double> momentum4(1.0, orientation);
//    return new Positron(energy_in, position4, momentum4);

//    return new Positron(energy_in, vec4<double>(0.0, position), vec4<double>(1.0, orientation));

    return std::move( std::unique_ptr<base_particle>( new Positron(energy_in, vec4<double>(0.0, position), vec4<double>(1.0, orientation)) ));
}




#ifdef __cplusplus
    }
#endif

