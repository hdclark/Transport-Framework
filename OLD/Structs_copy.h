//Structs.h - Classes and structs used throughout the project.

#ifndef STRUCTS_H_PROJECT_TRANSPORT
#define STRUCTS_H_PROJECT_TRANSPORT

#include "./MyMath.h"
#include "./Constants.h"

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
        double        energy;  //redundant, or convenient?

        vec4<double> X; //Contravariant four-position $X^{\mu}.$
        vec4<double> U; //Contravariant four-velocity $U^{\mu}.$
                        //Note: CContravariant four-momentum $P^{\mu} = m U^{\mu}.$

    public:
        //Constructors.
        base_particle();
        //base_particle(const vec4<double> &X_in, const vec4<double> &U_in);
        base_particle(const unsigned char &type_in, const double &mass_in, const double &charge_in, const double &energy_in);
        base_particle(const unsigned char &type_in, const double &mass_in, const double &charge_in, const double &energy_in, const vec4<double> &X_in, const vec4<double> &U_in); 

        //Methods - non-virtual.
        unsigned char get_type(void);
        double get_mass(void);
        double get_charge(void);

        vec3<double> get_position3(void);
        void set_position3(const vec3<double> &);
        vec3<double> get_momentum3(void);
        void set_momentum3(const vec3<double> &);

/*
        vec4<double> get_position4(void);
        void set_position4(const vec4<double> &);
        vec4<double> get_momentum4(void);
        void set_momentum4(const vec4<double> &);
*/
        //Methods - virtual.
        virtual double get_energy(void) =0;
        virtual void set_energy(const double &) =0;
        virtual double get_speed(void) =0;

};


#endif
