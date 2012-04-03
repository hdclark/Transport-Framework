//Transport.cc - The main entrypoint into the code.

#include <dlfcn.h> //Used for dlopen, etc.. Dynamic library loading.

#include <iostream>
#include <vector>
#include <string>

#include <random>     //We use this for PRNG's.
#include <functional> //Useful for binding ("currying") functions. Used for PRNG.

#include "./Misc.h"  //Various utilities. FUNCINFO, FUNCERR, etc..
#include "./Dynamic_Loading.h" //Routines for dynamically loading shared libraries.
#include "./Typedefs.h"  //Typedefs for the dynamic libraries. These let us slightly more safely cast function pointers.

bool VERBOSE = false;
const std::string VERSION("0.0.1 - Extreme Beta Version - Use at your own risk!");

long int random_seed = 123456;


int main(int argc, char *argv[]){
    
    std::vector<std::string> libraries;
    libraries.push_back("/home/hal/Dropbox/Project - Transport/Photons.so");
    libraries.push_back("/home/hal/Dropbox/Project - Transport/Random_MT.so");

    std::vector<void *> open_libraries;  //Keeps track of opened libraries. We need to keep them open until we are done.
  

    for(std::string library_fullpath : libraries){
        //Load in all libraries in the libraries string vector.
        //Program will simply halt if the file is not found in any of the usual places!
        void *loaded_library = load_library(library_fullpath);
    
        if(loaded_library != NULL){
            open_libraries.push_back( loaded_library );
  
            std::string FileType;
            std::string ParticleType;

 
            //First, enforce our verbosity level on the module. NOTE: We will still (possibly) get verbose messages prior to calling this. No way around this..
            if(check_for_item_in_library( loaded_library, "toggle_verbosity")){
                FUNCTION_toggle_verbosity_t loaded_function = reinterpret_cast<FUNCTION_toggle_verbosity_t>(load_item_from_library(loaded_library, "toggle_verbosity") );
                loaded_function( VERBOSE );
            } 

            //Load the file type identifier string.
            if(check_for_item_in_library( loaded_library, "FILE_TYPE")){
                std::string file_type = *reinterpret_cast<std::string *>(load_item_from_library(loaded_library, "FILE_TYPE"));
                std::cout << "Found file type identifier string: " << file_type << std::endl;
            }
 
            //Load the particle type identifier string.
            if(check_for_item_in_library( loaded_library, "PARTICLE_TYPE")){
                std::string *particle_type = reinterpret_cast<std::string *>(load_item_from_library(loaded_library, "PARTICLE_TYPE"));
                std::cout << "Found particle type identifier string: " << *particle_type << std::endl;
            }   
         
        
            //Load the particle type identifier string.
            if(check_for_item_in_library( loaded_library, "PARTICLE_TYPE")){
                std::string particle_type = *reinterpret_cast<std::string *>(load_item_from_library(loaded_library, "PARTICLE_TYPE"));
                std::cout << "Found particle type identifier string: " << particle_type << std::endl;
            }
        
            //Load in a void function.
            if(check_for_item_in_library( loaded_library, "some_void_function")){
                typedef void (*SomeFunction_t)(void);
                SomeFunction_t loaded_function = reinterpret_cast<SomeFunction_t>(load_item_from_library(loaded_library, "some_void_function") );
                loaded_function();
            }
        
        
            //Load in a double function.
            if(check_for_item_in_library( loaded_library, "some_double_function") ){
                typedef double (*SomeFunction_t)(double, double);
                SomeFunction_t loaded_function = reinterpret_cast<SomeFunction_t>(load_item_from_library(loaded_library, "some_double_function") );
                std::cout << " Calling function gives: " << loaded_function(2.345, 7.6875) << std::endl;
            }
        
        
            //Load in a vector function.
            if(check_for_item_in_library( loaded_library, "some_vector_function") ){
                typedef std::vector<double> (*SomeFunction_t)(double *);
                SomeFunction_t loaded_function = reinterpret_cast<SomeFunction_t>(load_item_from_library(loaded_library, "some_vector_function") );
        
                double dumb_number = 102458.654;
                std::vector<double> output = loaded_function(&dumb_number);
                std::cout << " Calling function gives: " << output[0] << std::endl;
            }
    
        }else{
            FUNCWARN("Unable to load library \"" << library_fullpath << "\". Proceeding anyways");
        }

    }   
 

    for(void *i : open_libraries ){
        close_library(i);
    }

    return 0;
}
