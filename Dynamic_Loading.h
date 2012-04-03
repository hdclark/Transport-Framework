//Dynamic_Loading.h

#ifndef DYNAMIC_LOADING_H_TRANSPORT
#define DYNAMIC_LOADING_H_TRANSPORT

#include <iostream>
#include <string>

#include "./Misc.h"  //Various utilities. FUNCINFO, FUNCERR, etc..
#include "./Dynamic_Loading.h"

//  Correct usage syntax is:
//
//funcp = (int (*)(int)) dlsym(...); 
//
//  or
//
//funcp = (int (*)(int)) load_item_from_library(....);
//
//  ala:
//     "The cast on the left-hand side is functionally useless and actually 
//      creates a second problem (the result of a cast is not an lvalue). 
//      (Note that in C++ you can't use a reinterpret_cast, since that's 
//      explicitly forbidden for conversion between function pointers and data 
//      pointers; you must use the old-style C cast.) "
//    from http://unix.derkeiler.com/Newsgroups/comp.unix.programmer/2004-05/0241.html.
//



void * load_library(std::string library_name);

void close_library(void *loaded_library);

void * load_item_from_library(void *loaded_library, std::string item_name);

bool check_for_item_in_library(void *loaded_library, std::string item_name);



//----------------------------------------------------------------------------------------------------
//------------------------------------------ Helper Macros -------------------------------------------
//----------------------------------------------------------------------------------------------------
//
//    FIXME FIXME FIXME	        WIP       FIXME FIXME FIXME
//
//  See http://unix.derkeiler.com/Newsgroups/comp.unix.programmer/2004-05/0241.html   :(
//      (and the previous message in the thread)
//


//This macro reduces the -pedantic errors of compiling code that looks like this:
//        |----->  int (*funcp)(int, std::string); //Pointer to function with int and std::string arguments.
//                 (void *) funcp = dlsym(handle, symbol);
// which produces, using gcc -pedantic , warnings:
//        |-----> "ANSI C forbids the use of cast expressions as lvalues"
// or     |
//        |-----> "warning: ISO C++ forbids casting between pointer-to-function and pointer-to-object"
// to the warning-free variant:
//        |----->  int (*funcp)(int, std::string); //Pointer to function with int and std::string arguments.
//                 *(void **) (&funcp) = dlsym(handle, symbol);
//
// Usage is NOT required, and warnings will only show up with the -pedantic option (gcc 4.6.2.)
// ____Usage____ is:
//   turn:
//        |----->          FUNCTION_init_explicit_seed PRNG_seed  = reinterpret_cast<FUNCTION_init_explicit_seed>(load_item_from_library(loaded_library, "init_explicit_seed") );
//   into:
//        |-----> FUNCTION_init_explicit_seed DYN_PED( PRNG_seed) = reinterpret_cast<FUNCTION_init_explicit_seed>(load_item_from_library(loaded_library, "init_explicit_seed") );
//
//  and say goodbye to those nasty pedantic warnings :)


//#ifndef DYN_PED
//    #define DYN_PED ( x )    *(void **)(&(#x)) 
//
//    typedef void sillywrapperforvoid;
//    #define DYN_PED ( x )    *(sillywrapperforvoid **)&
//
//    typedef *(void **)& DYN_PED
//
//    #define DYN_PED
//#endif


#endif

