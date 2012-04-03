//Misc.h
#ifndef MY_MISC_HDR_GRD_H
#define MY_MISC_HDR_GRD_H


#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <ctime> //For the gen_time_random().



//Error/Warning/Verbosity macros.

//Executable name variants.
#ifndef MY_BASIC_ERRFUNC_
    #define MY_BASIC_ERRFUNC_
    #define ERR( x )  {std::cerr << "--(E) " << argv[0]; \
                       std::cerr <<  ": " << x << ". Terminating program." << std::endl; \
                       std::cerr.flush();  \
                       std::exit(-1); }
#endif

#ifndef MY_BASIC_WARNFUNC_
    #define MY_BASIC_WARNFUNC_
    #define WARN( x )  {std::cout << "--(W) " << argv[0]; \
                        std::cout <<  ": " << x << "." << std::endl; \
                        std::cout.flush();  }
#endif

#ifndef MY_BASIC_INFOFUNC_
    #define MY_BASIC_INFOFUNC_
    #define INFO( x )  {std::cout << "--(I) " << argv[0]; \
                        std::cout <<  ": " << x << "." << std::endl; \
                        std::cout.flush();  }
#endif


//Function name variants.
#ifdef __GNUC__ //If using gcc..
    #ifndef __PRETTY_FUNCTION__
        #define __PRETTY_FUNCTION__ __func__
    #endif
#endif //__GNUC__

#ifndef __PRETTY_FUNCTION__   //(this is a fallback!)
    #define __PRETTY_FUNCTION__ '(filename not available)'
#endif

#ifndef FUNCMY_BASIC_ERRFUNC_
    #define FUNCMY_BASIC_ERRFUNC_
    #define FUNCERR( x )  {std::cerr << "--(E) In function: " << __PRETTY_FUNCTION__; \
                           std::cerr <<  ": " << x << ". Terminating program." << std::endl; \
                           std::cerr.flush();  \
                           std::exit(-1); }
#endif

#ifndef FUNCMY_BASIC_WARNFUNC_
    #define FUNCMY_BASIC_WARNFUNC_
    #define FUNCWARN( x )  {std::cout << "--(W) In function: " << __PRETTY_FUNCTION__; \
                            std::cout <<  ": " << x << "." << std::endl; \
                            std::cout.flush();  }
#endif


#ifndef FUNCMY_BASIC_INFOFUNC_
    #define FUNCMY_BASIC_INFOFUNC_
    #define FUNCINFO( x )  {std::cout << "--(I) In function: " << __PRETTY_FUNCTION__; \
                            std::cout <<  ": " << x << "." << std::endl; \
                            std::cout.flush();  }
#endif


//Convenience Macros.

#ifndef isininc
    //Inclusive_in_range()      isininc( 0, 10, 100) == true
    //                          isininc( 0, 100, 10) == false
    //                          isininc( 0, 10, 10)  == true
    //                          isininc( 10, 10, 10) == true
    #define isininc( A, x, B ) (((x) >= (A)) && ((x) <= (B)))
#endif


void gen_random(std::string *s, const int len);
void seed_time(void);
std::string inttostring(long int number);
std::string floattostring(float number);


template <class T>  std::string Xtostring(T numb);

template <class T>  T stringtoX(std::string text);


/*
void gen_random(std::string *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    (*s).clear();
    for(int i=0; i<len; ++i)   (*s) += alphanum[rand()%(sizeof(alphanum)-1)];
    return;
}
void seed_time(void){
    //Seed the rand() function with the current time.
    srand(time(NULL));
    return;
}

std::string inttostring(long int number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}

std::string floattostring(float number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}


template <class T>
std::string Xtostring(T numb){
    std::stringstream ss;
    ss << numb;
    return ss.str();
}
*/

unsigned long simple_hash(std::string &str);


#endif
