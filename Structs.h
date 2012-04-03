//Structs.h - Classes and structs used throughout the project.

#ifndef STRUCTS_H_PROJECT_TRANSPORT
#define STRUCTS_H_PROJECT_TRANSPORT

#include <vector>

#include "./MyMath.h"
#include "./Constants.h"
#include "./Typedefs.h"

//Contravariant four-vector class. 
template <class T>
class vec4 {
    public:
        T E;         //   <----  E/c
        vec3<T> p;   //   <----  \vec{p}
    
        //Constructors.
        vec4();
        vec4(T a, T px, T py, T pz);
        vec4(T e, const vec3<T> &p);
    
    
/*  //Implement these as needed. See MyMath.cc implementation of vec3 for reference.
    
        //Operators - vec4 typed.
        vec4 & operator=(const vec4 &);
    
        vec4   operator+(const vec4 &);
        vec4 & operator+=(const vec4 &);
        vec4   operator-(const vec4 &);
        vec4 & operator-=(const vec4 &);
    
        bool operator==(const vec4 &);
    
        //Operators - T typed.
        vec4   operator*(const T);
        vec4 & operator*=(const T);
        vec4   operator/(const T);
        vec4 & operator/=(const T);
*/

};


//This is a logging structure for keeping track of exactly what happens on a per-particle basis.
//
//Only really useful for post-simulation analysis! These should basically ONLY be created anonymously.
class an_interaction {
    public:
        unsigned char interaction;   //The type of interaction.
        unsigned char material;      //Which material the interaction occurs in.
        double energy;               //The energy the particle has immediately after the interaction occurs. (Note: we log particle creation)
        vec3<double> position;       //Where the event occurs.
        

        an_interaction();
        an_interaction(const unsigned char &in_interaction, const unsigned char &in_material, const double &in_E, const vec3<double> &in_pos);

};


//Abstract base particle class - holds info and methods for handling particles.
//
//Usage: Derive a class specifically for a specific particle and provide implementations of 
// the virtual functions.
//
//Why is this useful? We can treat all derived particles homogenously using polymorphism!
//
class base_particle {
    protected:
        
        unsigned char type;   //photon, electron, etc.. ?
        double        mass;   
        double        charge;
        double        energy;  //Total energy E, such that $E = \gamma mc^{2}$ or $E = h\nu$. NOT kinetic T!

        vec4<double> X; //Contravariant four-position $X^{\mu}.$
        vec4<double> U; //Contravariant four-velocity $U^{\mu}.$
                        //Note: Contravariant four-momentum $P^{\mu} = m U^{\mu}.$
                        //    thus the three-momentum $\vec{p} = \gamma m \vec{v}.$

    public:
        //Logging-related things.
        std::vector< an_interaction > Interactions;  //Keeps a list of the interactions the particle has undergone.


        //Constructors.
        base_particle();
        //base_particle(const vec4<double> &X_in, const vec4<double> &U_in);
        base_particle(const unsigned char &type_in, const double &mass_in, const double &charge_in, const double &energy_in);
        base_particle(const unsigned char &type_in, const double &mass_in, const double &charge_in, const double &energy_in, const vec4<double> &X_in, const vec4<double> &U_in); 

        //Methods - non-virtual.
        unsigned char get_type(void) const;
        double get_mass(void) const;
        double get_charge(void) const;

        vec3<double> get_position3(void);
        void set_position3(const vec3<double> &);
        vec3<double> get_relativistic_three_momentum3(void);
        void set_relativistic_three_momentum3(const vec3<double> &);

/*
        vec4<double> get_position4(void);
        void set_position4(const vec4<double> &);
        vec4<double> get_momentum4(void);
        void set_momentum4(const vec4<double> &);
*/
        //Methods - virtual.
        virtual double get_energy(void) const =0;
        virtual void set_energy(const double &) =0;
        virtual double get_speed(void) const =0;

};



struct Functions {

    //Uniformly-random orientation generator.
    FUNCTION_random_orientation    get_random_orientation;

    //Uniformly-random, clamped pseudo-random number generator / iterator.
    FUNCTION_PRNG_source           PRNG_source;

    //Returns the beam source outlet (ie. the source point.)
    FUNCTION_get_position          beam_position;

    //A particle sink for transfering control of a particle to the memory handler.
    FUNCTION_particle_sink         particle_sink;

    //Particle factory functions.
    FUNCTION_particle_factory      electron_factory;
    FUNCTION_particle_factory      photon_factory;
    FUNCTION_particle_factory      positron_factory;

    //Returns the char value corresponding to the material at a point in space.
    FUNCTION_geometry_type         which_material;

    //Generic particle graveyard used for logging. 
    FUNCTION_particle_graveyard    particle_graveyard;

    //Generic logging facilities.
    FUNCTION_generic_logging       generic_logging;

    //Quantities required for computing Kerma and/or Dose outside of Transport.cc.
    FUNCTION_average_energy_X      photon_average_energy_absorbed;
    FUNCTION_average_energy_X      photon_average_energy_transferred;
    FUNCTION_mass_coefficient_X    photon_mass_coefficient_transfer;
    FUNCTION_mass_coefficient_X    photon_mass_coefficient_absorption;
    FUNCTION_mass_coefficient_X    photon_mass_coefficient_total;

    //Voxels.
    FUNCTION_accumulate_slowdown   voxel_accumulation; 
    FUNCTION_voxel_localdump       voxel_localdump;
};


#endif
