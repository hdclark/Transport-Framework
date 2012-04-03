
#include <cmath>
#include <iostream>
#include <fstream>


#include "./Misc.h"

#include "./MyMath.h"
#include "./Structs.h"
#include "./Constants.h"

//Forward declarations.
template <class T>     class vec4;


//Constructors.
template <class T>     vec4<T>::vec4() : E((T)(0)), p() {   /*E=(T)(0);   p=vec3<T>(); */ }
template vec4<int>::vec4(void);
template vec4<float>::vec4(void);
template vec4<double>::vec4(void);

template <class T>     vec4<T>::vec4(T e, T px, T py, T pz) : E(e), p(px,py,pz) { }
template vec4<int>::vec4(int, int, int, int);
template vec4<float>::vec4(float, float, float, float);
template vec4<double>::vec4(double, double, double, double);

template <class T>     vec4<T>::vec4(T e, const vec3<T> &pin) : E(e), p(pin) { }
template vec4<int>::vec4(int, const vec3<int> &);
template vec4<float>::vec4(float, const vec3<float> &);
template vec4<double>::vec4(double, const vec3<double> &);






/*

template <class T>
std::ostream & operator<<( std::ostream &out, const vec3<T> &L ) {
  out << "("<< L.x << ", " << L.y << ", " << L.z <<")";
  return out;
}

template <class T>
std::istream &operator>>( std::istream &in, vec3<T> &L ) {
  //... << "("  << L.x << ", " << L.y << ", " <<  L.z  <<  ")";
  //We have at least TWO options here. We can use a method which is compatible
  // with the ( , , ) notation, or we can ask for straight-up numbers. 
  //We will discriminate here based on what 'in' is.
  if(in != std::cin){
      char grbg;
      //... << "("  << L.x << ", " << L.y << ", " <<  L.z  <<  ")";
      in    >> grbg >> L.x >> grbg >> L.y >> grbg >>  L.z  >> grbg;
  }else  in >> L.x >> L.y >> L.z;
  return in;
}
    
template <class T>
vec3<T> & vec3<T>::operator=(const vec3<T> &rhs) {
    //Check if it is itself.
    if (this == &rhs) return *this; 
    (*this).x = rhs.x;    (*this).y = rhs.y;    (*this).z = rhs.z;
     return *this;
}

template vec3<int> & vec3<int>::operator=(const vec3<int> &rhs);
template vec3<float> & vec3<float>::operator=(const vec3<float> &rhs);
template vec3<double> & vec3<double>::operator=(const vec3<double> &rhs);


template <class T>
vec3<T> vec3<T>::operator+(const vec3<T> &rhs) {
    vec3<T> temp;
    temp.x += rhs.x;    temp.y += rhs.y;    temp.z += rhs.z;
    return temp;
}
template <class T>
vec3<T> & vec3<T>::operator+=(const vec3<T> &rhs) {
    (*this).x += rhs.x;    (*this).y += rhs.y;    (*this).z += rhs.z;
    return *this;
}

template vec3<int> & vec3<int>::operator+=(const vec3<int> &rhs);
template vec3<float> & vec3<float>::operator+=(const vec3<float> &rhs);
template vec3<double> & vec3<double>::operator+=(const vec3<double> &rhs);


template <class T>
vec3<T> vec3<T>::operator-(const vec3<T> &rhs) {
    vec3<T> temp;
    temp.x -= rhs.x;    temp.y -= rhs.y;    temp.z -= rhs.z;
    return temp;
}
template <class T>
vec3<T> & vec3<T>::operator-=(const vec3<T> &rhs) {
    (*this).x -= rhs.x;    (*this).y -= rhs.y;    (*this).z -= rhs.z;
    return *this;
}

template vec3<int> & vec3<int>::operator-=(const vec3<int> &rhs);
template vec3<float> & vec3<float>::operator-=(const vec3<float> &rhs);
template vec3<double> & vec3<double>::operator-=(const vec3<double> &rhs);


template <class T>
vec3<T> vec3<T>::operator*(const T rhs) {
    vec3<T> temp;
    temp.x *= rhs;    temp.y *= rhs;    temp.z *= rhs;
    return temp;
}
template <class T>
vec3<T> & vec3<T>::operator*=(const T rhs) {
    (*this).x *= rhs;    (*this).y *= rhs;    (*this).z *= rhs;
     return *this;
}

template vec3<int> & vec3<int>::operator*=(const int rhs);
template vec3<float> & vec3<float>::operator*=(const float rhs);
template vec3<double> & vec3<double>::operator*=(const double rhs);


template <class T>
vec3<T> vec3<T>::operator/(const T rhs) {
    vec3<T> temp;
    temp.x /= rhs;    temp.y /= rhs;    temp.z /= rhs;
    return temp;
}
template <class T>
vec3<T> & vec3<T>::operator/=(const T rhs) {
    (*this).x /= rhs;    (*this).y /= rhs;    (*this).z /= rhs;
     return *this;
}

template vec3<int> & vec3<int>::operator/=(const int rhs);
template vec3<float> & vec3<float>::operator/=(const float rhs);
template vec3<double> & vec3<double>::operator/=(const double rhs);


template <class T>
bool vec3<T>::operator==(const vec3<T> &rhs){
    if( (x == rhs.x) && (y == rhs.y) && (z == rhs.z) ) return true;
    return false;
}

*/


//-------------------------------- an_interaction -------------------------------------
//Constructors.
an_interaction::an_interaction() : interaction(0),material(0),energy(0),position() { }
an_interaction::an_interaction(const unsigned char &in_interaction, const unsigned char &in_material, const double &in_E, const vec3<double> &in_pos) : interaction(in_interaction),material(in_material),energy(in_E),position(in_pos) { }



//-------------------------------- base_particle --------------------------------------
//Constructors.
base_particle::base_particle() : type(0),mass(0),charge(0),energy(0),X(),U() { }

base_particle::base_particle(const unsigned char &type_in, const double &mass_in, const double &charge_in, const double &energy_in) : type(type_in),mass(mass_in),charge(charge_in),energy(energy_in),X(),U() { }

//base_particle::base_particle(const vec4<double> &X_in, const vec4<double> &U_in) : type(0),mass(0),charge(0),energy(0),X(X_in),U(U_in) { }

base_particle::base_particle(const unsigned char &type_in, const double &mass_in, const double &charge_in, const double &energy_in, const vec4<double> &X_in, const vec4<double> &U_in) : type(type_in),mass(mass_in),charge(charge_in),energy(energy_in),X(X_in),U(U_in) { }



//Methods - non-virtual.
unsigned char base_particle::get_type(void) const { return type; }
double        base_particle::get_mass(void) const { return mass; }
double        base_particle::get_charge(void) const { return charge; }

vec3<double> base_particle::get_position3(void){ return X.p; }
void base_particle::set_position3(const vec3<double> &in){ X.p = in; return; }

//vec3<double> base_particle::get_relativistic_three_momentum3(void){ return vec3<double>( (U.p).x*mass,(U.p).y*mass,(U.p).z*mass ); }
vec3<double> base_particle::get_relativistic_three_momentum3(void){ 
    if(type == Particletype::Photon ){
        return (U.p)*energy;
    }
    return (U.p)*mass; 
}

void base_particle::set_relativistic_three_momentum3(const vec3<double> &in){
    if(type == Particletype::Photon ){
        U.p = in;
        if(energy < 1.0E-10) FUNCERR("Dangerously low-energy photon being created (E = " << energy << ")");
        U.p /= energy;    // THIS IS DANGEROUS. HOW DO WE GUARANTEE ENERGY IS ALREADY DEFINED?
        return;
    }
    U.p = in; 
    U.p /= mass;  //Note that this stores only the relativistic three-velocity $\vec{u} = \gamma \vec{v}.$
    return; 
}




//----------------------------- explicit instantiations -------------------------------
//template class vec4<double>;
