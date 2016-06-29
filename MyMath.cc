
#include <cmath>
#include <iostream>
#include <fstream>

#include "./MyMath.h"

//using namespace::std;


template <class T> class vec3;

//Constructors.
template <class T>    vec3<T>::vec3(){   x=(T)(0);   y=(T)(0);   z=(T)(0);  }
template vec3<int>::vec3(void);
template vec3<long int>::vec3(void);
template vec3<float>::vec3(void);
template vec3<double>::vec3(void);


template <class T>    vec3<T>::vec3(T a, T b, T c) : x(a), y(b), z(c) { }
template vec3<int>::vec3(int, int, int);
template vec3<long int>::vec3(long int, long int, long int);
template vec3<float>::vec3(float, float, float);
template vec3<double>::vec3(double, double, double);



//More general: (but is it needed?)
//template<class Ch,class Tr,class T>     std::basic_ostream<Ch,Tr> & operator<<( std::basic_ostream<Ch,Tr> &&out, const vec3<T> &L ){
//    out << "(" << L.x << ", " << L.y << ", " << L.z << ")";
//    return out;
//}
template <class T>    std::ostream & operator<<( std::ostream &out, const vec3<T> &L ) {
    out << "(" << L.x << ", " << L.y << ", " << L.z << ")";
    return out;
}
template std::ostream & operator<<(std::ostream &out, const vec3<int> &L );
template std::ostream & operator<<(std::ostream &out, const vec3<long int> &L );
template std::ostream & operator<<(std::ostream &out, const vec3<float> &L );
template std::ostream & operator<<(std::ostream &out, const vec3<double> &L );


template <class T> vec3<T> vec3<T>::unit(void){
    const T tot = sqrt(x*x + y*y + z*z);
    return vec3<T>(x/tot, y/tot, z/tot);
} 

template vec3<float> vec3<float>::unit(void);
template vec3<double> vec3<double>::unit(void);

template <class T> T vec3<T>::length(void){
    const T tot = sqrt(x*x + y*y + z*z);
    return tot;
}

template float vec3<float>::length(void);
template double vec3<double>::length(void);


template <class T>  T vec3<T>::distance(const vec3 &rhs){
    const T dist = sqrt((x-rhs.x)*(x-rhs.x) + (y-rhs.y)*(y-rhs.y) + (z-rhs.z)*(z-rhs.z));
    return dist;
}

template float vec3<float>::distance(const vec3<float> &rhs);
template double vec3<double>::distance(const vec3<double> &rhs);


template <class T>    std::istream &operator>>( std::istream &in, vec3<T> &L ) {
  char grbg;
  //... << "("  << L.x << ", " << L.y << ", " <<  L.z  <<  ")";
  in    >> grbg >> L.x >> grbg >> L.y >> grbg >>  L.z  >> grbg;
  return in;
}
    

template <class T>    vec3<T> & vec3<T>::operator=(const vec3<T> &rhs) {
    //Check if it is itself.
    if (this == &rhs) return *this; 
    (*this).x = rhs.x;    (*this).y = rhs.y;    (*this).z = rhs.z;
     return *this;
}
template vec3<int> & vec3<int>::operator=(const vec3<int> &rhs);
template vec3<long int> & vec3<long int>::operator=(const vec3<long int> &rhs);
template vec3<float> & vec3<float>::operator=(const vec3<float> &rhs);
template vec3<double> & vec3<double>::operator=(const vec3<double> &rhs);


template <class T>    vec3<T> vec3<T>::operator+(const vec3<T> &rhs) {
    vec3<T> temp;
    temp.x += rhs.x;    temp.y += rhs.y;    temp.z += rhs.z;
    return temp;
}

template <class T>    vec3<T> & vec3<T>::operator+=(const vec3<T> &rhs) {
    (*this).x += rhs.x;    (*this).y += rhs.y;    (*this).z += rhs.z;
    return *this;
}
template vec3<int> & vec3<int>::operator+=(const vec3<int> &rhs);
template vec3<long int> & vec3<long int>::operator+=(const vec3<long int> &rhs);
template vec3<float> & vec3<float>::operator+=(const vec3<float> &rhs);
template vec3<double> & vec3<double>::operator+=(const vec3<double> &rhs);


template <class T>
vec3<T> vec3<T>::operator-(const vec3<T> &rhs) {
    vec3<T> temp;
    temp.x -= rhs.x;    temp.y -= rhs.y;    temp.z -= rhs.z;
    return temp;
}

template <class T>    vec3<T> & vec3<T>::operator-=(const vec3<T> &rhs) {
    (*this).x -= rhs.x;    (*this).y -= rhs.y;    (*this).z -= rhs.z;
    return *this;
}
template vec3<int> & vec3<int>::operator-=(const vec3<int> &rhs);
template vec3<long int> & vec3<long int>::operator-=(const vec3<long int> &rhs);
template vec3<float> & vec3<float>::operator-=(const vec3<float> &rhs);
template vec3<double> & vec3<double>::operator-=(const vec3<double> &rhs);

//------------------------------ overloaded native-types -----------------------------

/*
template <class T>    vec3<T> vec3<T>::operator*(const T rhs) {
    return vec3<T>(x*rhs,y*rhs,z*rhs);
}
template <class T>    vec3<T> & vec3<T>::operator*=(const T rhs) {
    (*this).x *= rhs;    (*this).y *= rhs;    (*this).z *= rhs;
     return *this;
}
template vec3<int> & vec3<int>::operator*=(const int rhs);
template vec3<float> & vec3<float>::operator*=(const float rhs);
template vec3<double> & vec3<double>::operator*=(const double rhs);



template <class T>    vec3<T> vec3<T>::operator/(const T rhs) {
    return vec3<T>(x/rhs,y/rhs,z/rhs);
}
template <class T>    vec3<T> & vec3<T>::operator/=(const T rhs) {
    (*this).x /= rhs;    (*this).y /= rhs;    (*this).z /= rhs;
     return *this;
}
template vec3<int> & vec3<int>::operator/=(const int rhs);
template vec3<float> & vec3<float>::operator/=(const float rhs);
template vec3<double> & vec3<double>::operator/=(const double rhs);
*/

//--------

template <class T>    vec3<T> vec3<T>::operator*(const T &rhs) {
    return vec3<T>(x*rhs,y*rhs,z*rhs);
}
template vec3<int> vec3<int>::operator*(const int &rhs);
template vec3<long int> vec3<long int>::operator*(const long int &rhs);
template vec3<float> vec3<float>::operator*(const float &rhs);
template vec3<double> vec3<double>::operator*(const double &rhs);

template <class T>    vec3<T> & vec3<T>::operator*=(const T &rhs) {
    (*this).x *= rhs;    (*this).y *= rhs;    (*this).z *= rhs;
     return *this;
}
template vec3<int> & vec3<int>::operator*=(const int &rhs);
template vec3<long int> & vec3<long int>::operator*=(const long int &rhs);
template vec3<float> & vec3<float>::operator*=(const float &rhs);
template vec3<double> & vec3<double>::operator*=(const double &rhs);



template <class T>    vec3<T> vec3<T>::operator/(const T &rhs) {
    return vec3<T>(x/rhs,y/rhs,z/rhs);
}
template vec3<int> vec3<int>::operator/(const int &rhs);
template vec3<long int> vec3<long int>::operator/(const long int &rhs);
template vec3<float> vec3<float>::operator/(const float &rhs);
template vec3<double> vec3<double>::operator/(const double &rhs);

template <class T>    vec3<T> & vec3<T>::operator/=(const T &rhs) {
    (*this).x /= rhs;    (*this).y /= rhs;    (*this).z /= rhs;
     return *this;
}
template vec3<int> & vec3<int>::operator/=(const int &rhs);
template vec3<long int> & vec3<long int>::operator/=(const long int &rhs);
template vec3<float> & vec3<float>::operator/=(const float &rhs);
template vec3<double> & vec3<double>::operator/=(const double &rhs);



template <class T>    bool vec3<T>::operator==(const vec3<T> &rhs){
    return ( (x == rhs.x) && (y == rhs.y) && (z == rhs.z) );
}
template bool vec3<int>::operator==(const vec3<int> &rhs);
template bool vec3<long int>::operator==(const vec3<long int> &rhs);
template bool vec3<float>::operator==(const vec3<float> &rhs);
template bool vec3<double>::operator==(const vec3<double> &rhs);

//------------------------------ explicit instantiation -----------------------------

//template class vec3<double>;





//This is a function for rotation unit vectors in some plane. It requires angles to describe the plane of rotation, angle of rotation. 
// It alo requires a unit vector with which to rotate the plane about.
vec3<double> rotate_unit_vector_in_plane(const vec3<double> &A, const double &theta, const double &R){
    // A    --- The unit vector which defines the central axis of the plane. We rotate about this vector to make a plane.
    // theta -- The angle of rotation for a unit vector within the plane.
    // R    --- An angle from [0:2*pi] which specifies the second axis of the plane (along with A.)

    //###########################################################################################
    //## This method is NOT ideal. It is expensive AND brittle. Fix it with a better solution. ##
    //###########################################################################################

    std::complex<double> a1 = (fabs(A.x) > 1E-11) ? ( (fabs(A.x) < (1.0-1E-10)) ? A.x :  A.x - 1E-10 ) : 1E-11 + A.x;    // ~~ A.x
    std::complex<double> a2 = (fabs(A.y) > 1E-11) ? ( (fabs(A.y) < (1.0-1E-10)) ? A.y :  A.y - 1E-10 ) : 1E-11 + A.y;    // ~~ A.y
    std::complex<double> a3 = (fabs(A.z) > 1E-11) ? ( (fabs(A.z) < (1.0-1E-10)) ? A.z :  A.z - 1E-10 ) : 1E-11 + A.z;    // ~~ A.z

    static const std::complex<double> i(0.0,1.0);

    std::complex<double> p, t; //Angles.

    if(fabs(A.z) < 0.75){ //Handles special cases. Doesn't do so safely, though!

    //Now, given the rotation angle and the unit vector coordinates of A, we generate a unit vector in the plane orthogonal to A.   
    p  = (R > 1E-11) ? R : 1E-11 + R;  // ~~ R

    //Two solutions for t when fixing p. Pick one (I think they correspond to the plus/minus orientation, which should be irrelevant here.)
    t  = -i*log(-1.0*pow(a2*pow(M_E,2.0*i*p)+i*a1*pow(M_E,2.0*i*p)-1.0*a2-1.0*i*a1,-1.0)*pow(pow(a3,2.0)*pow(M_E,4.0*i*p)+pow(a2,2.0)*pow(M_E,4.0*i*p)+pow(a1,2.0)*pow(M_E,4.0*i*p)+2.0*pow(a3,2.0)*pow(M_E,2.0*i*p)-2.0*pow(a2,2.0)*pow(M_E,2.0*i*p)-2.0*pow(a1,2.0)*pow(M_E,2.0*i*p)+pow(a3,2.0)+pow(a2,2.0)+pow(a1,2.0),0.5)+a3*pow(M_E,2.0*i*p)*pow(a2*pow(M_E,2.0*i*p)+i*a1*pow(M_E,2.0*i*p)-1.0*a2-1.0*i*a1,-1.0)+a3*pow(a2*pow(M_E,2.0*i*p)+i*a1*pow(M_E,2.0*i*p)-1.0*a2-1.0*i*a1,-1.0));

    //t  = -i*log(pow(a2*pow(M_E,2.0*i*p)+i*a1*pow(M_E,2.0*i*p)-1.0*a2-1.0*i*a1,-1.0)*pow(pow(a3,2.0)*pow(M_E,4.0*i*p)+pow(a2,2.0)*pow(M_E,4.0*i*p)+pow(a1,2.0)*pow(M_E,4.0*i*p)+2.0*pow(a3,2.0)*pow(M_E,2.0*i*p)-2.0*pow(a2,2.0)*pow(M_E,2.0*i*p)-2.0*pow(a1,2.0)*pow(M_E,2.0*i*p)+pow(a3,2.0)+pow(a2,2.0)+pow(a1,2.0),0.5)+a3*pow(M_E,2.0*i*p)*pow(a2*pow(M_E,2.0*i*p)+i*a1*pow(M_E,2.0*i*p)-1.0*a2-1.0*i*a1,-1.0)+a3*pow(a2*pow(M_E,2.0*i*p)+i*a1*pow(M_E,2.0*i*p)-1.0*a2-1.0*i*a1,-1.0));


    }else{
    
        t = R;

        //We are going to use least-significant bit selection here. Ugh... Of COURSE this is not portable. Endianness breaks it, if not something else.
        union {
            double the_number;
            char   asChars[sizeof(double)];
        };
        the_number = R;

        //This time we cannot simply ignore one of the solutions, lest we have a one-sided universe...
     
        // http://stackoverflow.com/questions/4743115/how-do-i-use-bitwise-operators-on-a-double-on-c
 
        //if(asChars[sizeof(double) - 1] & 0x1){
        if(asChars[0] & 0x1){  //Least significant bit selection. Sorry, if you are reading this, for such a shit solution :(
            p = -i*log(-1.0*pow(a2*pow(M_E,2.0*i*t)*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0)+i*a1*pow(M_E,2.0*i*t)*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0)+2.0*a3*pow(M_E,i*t)*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0)-1.0*a2*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0)+i*a1*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0),0.5));
        }else{
            p = -0.5*i*log(a2*pow(M_E,2.0*i*t)*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0)+i*a1*pow(M_E,2.0*i*t)*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0)+2.0*a3*pow(M_E,i*t)*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0)-1.0*a2*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0)+i*a1*pow(a2*pow(M_E,2.0*i*t)+i*a1*pow(M_E,2.0*i*t)-2.0*a3*pow(M_E,i*t)-1.0*a2+i*a1,-1.0));
        }
    }

    double u1;
    double u2;
    double u3;

    u1 = ( cos(t)*sin(p) ).real();
    u2 = ( sin(t)*sin(p) ).real();
    u3 = ( cos(p) ).real();

    //Note that taking the real part messes up the normalization, leading to some fairly funky 'strands' in the rotated vector.
    // If you sample theta but leave A and R static, you should see these strands if you remove the following renormalization.
    const double utot = sqrt( u1*u1 + u2*u2 + u3*u3 );
    u1 /= utot;
    u2 /= utot;
    u3 /= utot;

    //Now we rotate by the angle provided.
    const double out_x = cos(theta)*A.x + sin(theta)*u1;
    const double out_y = cos(theta)*A.y + sin(theta)*u2;
    const double out_z = cos(theta)*A.z + sin(theta)*u3;

    //Now we ship out the vector.
    return vec3<double>( out_x, out_y, out_z );

}



