#include <iostream>
#include <cmath>
#include <complex>
#include <random>
#include <fstream>

#include "./MyMath.h"



//This program tests out the rotate_about_a_unit_vector_in_a_plane function. Draws some loops in various points. Looks for nans, infs, and garbage.

int main(int argc, char **argv){


    std::mt19937 random_engine;
    std::uniform_real_distribution<> random_distribution(0.0, 1.0);
    std::random_device rd; //Generates a hardware-generated random seed.
    random_engine.seed( rd() );
    // Call like: random_distribution(random_engine); 



    vec3<double> test1(0.0,0.0,-1.0);
    vec3<double> test2 = (rotate_unit_vector_in_plane(test1, 0.5*M_PI, 0.5*M_PI)); 

    std::cout << "The first vector was " << test1 << std::endl;
    std::cout << "   and the second is " << test2 << std::endl;

    std::fstream Filep;
    Filep.open("/tmp/mathtest", std::ios::out );
    if(!Filep.good()){
        std::cout << "Unable to open profile output file. Exiting." << std::endl;
        return -1;
    }
    Filep << "#   x   y   z " << std::endl;
    for(size_t j=0; j<10000; ++j){
        vec3<double> test1( 1.2, 2.3, 3.4 );
        test1.unit();
        vec3<double> test2 = (rotate_unit_vector_in_plane(test1, 1.234, random_distribution(random_engine)*2.0*M_PI));

        Filep << test2 << std::endl;
    }

    for(size_t j=0; j<10000; ++j){
        vec3<double> test1( 1.2, 2.3, 3.4 );
        test1.unit();
        vec3<double> test2 = (rotate_unit_vector_in_plane(test1, 1.75, random_distribution(random_engine)*2.0*M_PI));

        Filep << test2 << std::endl;
    }

    for(size_t j=0; j<10000; ++j){
        vec3<double> test1( 1.2, 2.3, 3.4 );
        test1.unit();
        vec3<double> test2 = (rotate_unit_vector_in_plane(test1, 2.15, random_distribution(random_engine)*2.0*M_PI));

        Filep << test2 << std::endl;
    }


    for(size_t j=0; j<10000; ++j){
        vec3<double> test1( -4.2, -0.3, 2.4 );
        test1.unit();
        vec3<double> test2 = (rotate_unit_vector_in_plane(test1, -1.234, random_distribution(random_engine)*2.0*M_PI));

        Filep << test2 << std::endl;
    }


    for(size_t j=0; j<10000; ++j){
        vec3<double> test1( 1.2, 2.3, 3.4 );
        test1.unit();
//        vec3<double> test2 = (rotate_unit_vector_in_plane(test1, random_distribution(random_engine)*M_PI, random_distribution(random_engine)*2.0*M_PI));
//        vec3<double> test2 = (rotate_unit_vector_in_plane(test1, random_distribution(random_engine)*M_PI, 1.23456));
        vec3<double> test2 = (rotate_unit_vector_in_plane(test1, 2.0*random_distribution(random_engine)*M_PI, 1.23456));

        Filep << test2 << std::endl;
    }



    Filep.close();


    return 0;
}
