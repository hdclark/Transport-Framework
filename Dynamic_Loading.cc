//Dynamic_Loading.cc - Routines which handle dynamically loading libraries. 
//
//These routines basically wrap the dlopen/dlsym/dlclose routines with extra error checking.
// They are not specialized - they simply return pointers and booleans. Do casting elsewhere!

#include <dlfcn.h> //Used for dlopen, etc.. Dynamic library loading.

#include <iostream>
#include <vector>
#include <string>

#include "./Misc.h"  //Various utilities. FUNCINFO, FUNCERR, etc..

#include "./Dynamic_Loading.h"


void * load_library(std::string library_name){
    dlerror(); //Flush previous error messages.
    void *loaded_library = dlopen(library_name.c_str(), RTLD_NOW);
    char *error_message = dlerror(); //Needs to be char*.
    if(error_message != NULL)  FUNCWARN("Dynamic file loader says: " << error_message);
    if(loaded_library == NULL) FUNCERR("Unable to load library: \"" << library_name << "\" - check if file exists");

    return loaded_library;
}

void close_library(void *loaded_library){
    if(loaded_library == NULL){
        FUNCWARN("Attempted to close a library which is not open. Ignoring.");
    }else{
        dlclose(loaded_library);
        //if(VERBOSE) FUNCINFO("Successfully closed library");
    }
    return;
}

void * load_item_from_library(void *loaded_library, std::string item_name){
    if(loaded_library == NULL){
        FUNCERR("Attempted to look inside a library which is not currently loaded");
    }else{
        dlerror();
        void *loaded_item = dlsym(loaded_library, item_name.c_str());
        char *error_message = dlerror(); //Needs to be char*.
    
        if(error_message != NULL) FUNCWARN("Dynamic file loader says: " << error_message);
        if(loaded_item   == NULL){
            FUNCERR("Unable to find item: \"" << item_name << "\".");
        }else{
            //if(VERBOSE) FUNCINFO("Found item: \"" << item_name << "\".");
        }
        return loaded_item;
    }
    return NULL;
}

bool check_for_item_in_library(void *loaded_library, std::string item_name){
    //Use this function to check if an item exists inside a library. One could do the same with the function
    // to directly load the item and then check the return value, but this is a logically cleaner way to 
    // do the same task. And there is no hope of dropping a NULL reference into a function cast accidentally.
    //
    //Returns: true if the item exists in the file, false otherwise.
    if(loaded_library == NULL){
        FUNCWARN("Attempted to look inside a library which is not currently loaded");
        return false;
    }else{
        dlerror();
        void *loaded_item = dlsym(loaded_library, item_name.c_str());
        char *error_message = dlerror(); //Needs to be char*.

        if(error_message != NULL){
            //FUNCWARN("Dynamic file loader says: " << error_message);
        }
        if(loaded_item   == NULL){
            //if(VERBOSE) FUNCWARN("Unable to find item: \"" << item_name << "\".");
            return false;
        }else{
            //if(VERBOSE) FUNCINFO("Found item: \"" << item_name << "\".");
            return true;
        }
    }
    return false;
}


/* ----- EXAMPLE USAGE ---------
    
    std::vector<std::string> libraries;
    libraries.push_back("/home/hal/Dropbox/Project - Transport/Photons.so");
    libraries.push_back("Photons.so");
    libraries.push_back("Electrons.so");


    //Load in all libraries in the libraries string vector.
    void *loaded_library = load_library(libraries[0]);


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


    close_library(loaded_library);
*/

