#ifndef _MYMATH_H_PROJECT_TRANSPORT
#define _MYMATH_H_PROJECT_TRANSPORT
#include <cmath>
#include <iostream>
#include <fstream>
#include <complex>
//using namespace::std;

//Simple, geometric three-vector. Nothing fancy!

template <class T>
class vec3 {
    public:
    T x, y, z;

    //Constructors.
    vec3();
    vec3(T a, T b, T c);

    //Operators - vec3 typed.
    vec3 & operator=(const vec3 &);

    vec3   operator+(const vec3 &);
    vec3 & operator+=(const vec3 &);
    vec3   operator-(const vec3 &);
    vec3 & operator-=(const vec3 &);

    bool operator==(const vec3 &);

    //Operators - T typed.
/*
    vec3   operator*(const T);
    vec3 & operator*=(const T);
    vec3   operator/(const T);
    vec3 & operator/=(const T);
*/
    vec3   operator*(const T &);
    vec3 & operator*=(const T &);
    vec3   operator/(const T &);
    vec3 & operator/=(const T &);


    //Other.
    // ---> normalization operator.
    vec3 unit();
    // ---> (pythagorean) length of vector.
    T length();
    // ---> (pythagorean) distance between vectors.
    T distance(const vec3 &);

    // ---> overloaded stream operator.
    template<class Y> friend std::ostream & operator << (std::ostream &, const vec3<Y> &);

};



//This is a function for rotation unit vectors in some plane. It requires angles to describe the plane of rotation, angle of rotation. 
// It also requires a unit vector with which to rotate the plane about.
vec3<double> rotate_unit_vector_in_plane(const vec3<double> &A, const double &theta, const double &R);


#endif
