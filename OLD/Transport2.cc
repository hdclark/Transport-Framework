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

#include "./MyMath.h" //vec3, etc..
#include "./Structs.h" //vec4, etc.. 

#include "./Constants.h"

bool VERBOSE = false;
const std::string VERSION("0.0.1 - Extreme Beta Version - Use at your own risk!");

//Passed in as arguments.
long int random_seed = 123456;
std::vector<void *> open_libraries;  //Keeps track of opened libraries. We need to keep them open until we are done.
unsigned char beam_type; //Which type of particle should come from the beam source. Types are listed in Constants.cc.


//----------------------------------------------------------------------------------------------------
//------------------------------------ Dynamically loaded functions ----------------------------------
//----------------------------------------------------------------------------------------------------
FUNCTION_PRNG_source          PRNG_source; //A pseudo-random number generator source/iterator function.
FUNCTION_energy_distribution  beam_energy_distribution; //The beam-source energy distribution. (Not collision distribution.)
FUNCTION_get_position         beam_position; //Returns the beam source outlet (ie. the source point.)
FUNCTION_set_position         set_beam_position; //Lets us adjust the beam source outlet (ie. the source point.)
FUNCTION_get_orientation      get_new_orientation; //Gets a new orientation unit vector for a particle ejected from the source outlet. (holds the angular distribution of the source).
FUNCTION_geometry_type        which_material; //Returns the char value corresponding to the material at a point in space.
FUNCTION_particle_factory     photon_factory; //Derived class factory function - creates a Photon class instance on the heap.


//----------------------------------------------------------------------------------------------------
//------------------------------------- Entry into program here --------------------------------------
//----------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]){

    //  INCOMING:      -libraries which need to be loaaded,
    //                 -seed value (if desired)
    //                 -verbosity
    //                 -beam type (what we want to come from the beam source)

    std::vector<std::string> libraries;
    //libraries.push_back("/home/hal/Dropbox/Project - Transport/lib_beams.so");
    libraries.push_back("./lib_photons.so");
    libraries.push_back("./lib_random_MT.so");
    libraries.push_back("./lib_water.so");
    libraries.push_back("./lib_beams.so");
    libraries.push_back("./lib_geometry.so");
 

    //Load in all libraries in the libraries string vector.
    //Program will simply halt if the file is not found in any of the usual places!
    for(std::string library_fullpath : libraries){
        void *loaded_library = load_library(library_fullpath);
    
        if(loaded_library != NULL){
            open_libraries.push_back( loaded_library );
            std::string FileType;
            std::string ParticleType;

            //--------------------------------- Set up generic module things ----------------------------------
            //First, enforce our verbosity level on the module. 
            // NOTE: We will still (possibly) get verbose messages prior to calling this. The way to fix this is not worth the effort.
            if(check_for_item_in_library( loaded_library, "toggle_verbosity")){
                FUNCTION_toggle_verbosity_t loaded_function = reinterpret_cast<FUNCTION_toggle_verbosity_t>(load_item_from_library(loaded_library, "toggle_verbosity") );
                loaded_function( VERBOSE );
            } 

            //Load the file type identifier string.
            if(check_for_item_in_library( loaded_library, "FILE_TYPE")){
                FileType = *reinterpret_cast<std::string *>(load_item_from_library(loaded_library, "FILE_TYPE"));
            }
 
            //Load the particle type identifier string.
            if(check_for_item_in_library( loaded_library, "PARTICLE_TYPE")){
                ParticleType = *reinterpret_cast<std::string *>(load_item_from_library(loaded_library, "PARTICLE_TYPE"));
            }   
        
 
            //------------------------------- Set up the random number generator -------------------------------
            if(FileType == "PRNG"){
                //Seed the generator. It will be randomly (non-reproduceably) seeded otherwise.
                if(check_for_item_in_library( loaded_library, "init_explicit_seed")){
                    FUNCTION_init_explicit_seed PRNG_seed = reinterpret_cast<FUNCTION_init_explicit_seed>(load_item_from_library(loaded_library, "init_explicit_seed") );
                    PRNG_seed(123456);
                }
   
                //Grab the source/iterator function.
                if(check_for_item_in_library( loaded_library, "source")){
                    PRNG_source = reinterpret_cast<FUNCTION_PRNG_source>(load_item_from_library(loaded_library, "source") );
                }

            //---------------------------------- Set up the beam geometry --------------------------------------
            }else if(FileType == "BEAM"){
                //Grab the beam position get/set functions.
                if(check_for_item_in_library( loaded_library, "get_position")){
                    beam_position = reinterpret_cast<FUNCTION_get_position>(load_item_from_library(loaded_library, "get_position"));
                }
                if(check_for_item_in_library( loaded_library, "set_position")){
                    set_beam_position = reinterpret_cast<FUNCTION_set_position>(load_item_from_library(loaded_library, "set_position"));
                }

                //Grab the beam orientation distribution function.
                if(check_for_item_in_library( loaded_library, "get_orientation")){
                    get_new_orientation = reinterpret_cast<FUNCTION_get_orientation>(load_item_from_library(loaded_library, "get_orientation"));
                }

            //------------------------------ Set up the geometry bounding boxes --------------------------------
            }else if(FileType == "GEOMETRY"){
                //Grab the material bounding box function.
                if(check_for_item_in_library( loaded_library, "geometry_type")){
                    which_material = reinterpret_cast<FUNCTION_geometry_type>(load_item_from_library(loaded_library, "geometry_type") );
                }


            //--------------------------------- Set up the photon functions ------------------------------------
            }else if(ParticleType == "PHOTON"){
                //Grab the energy distribution function.
                if(check_for_item_in_library( loaded_library, "energy_distribution")){
                    beam_energy_distribution = reinterpret_cast<FUNCTION_energy_distribution>(load_item_from_library(loaded_library, "energy_distribution") );
                }

                //Grab the Photon particle class factory function. (It polymorphs the base_particle class!)
                if(check_for_item_in_library( loaded_library, "particle_factory")){
                    photon_factory = reinterpret_cast<FUNCTION_particle_factory>(load_item_from_library(loaded_library, "particle_factory") );
                }


            //-------------------------------- Set up the electron functions -----------------------------------
            }else if(ParticleType == "ELECTRON"){



            }

        }else{
            FUNCWARN("Unable to load library \"" << library_fullpath << "\". Proceeding");
        }

    }   
 


    //----------------------------------------------------------------------------------------------------
    //----------------------------- Check to ensure it is safe to continue -------------------------------
    //----------------------------------------------------------------------------------------------------
    if( (beam_energy_distribution == NULL ) 
          || (PRNG_source == NULL )
          || (beam_position == NULL )
          || (set_beam_position == NULL )
          || (which_material == NULL )
          || (photon_factory == NULL )
      ){

        FUNCERR("Do not have necessary information to continue - check modules were loaded properly");
 
    }


    //----------------------------------------------------------------------------------------------------
    //----------------------------------- Bind functions, if desired -------------------------------------
    //----------------------------------------------------------------------------------------------------
    //auto new_beam_energy = std::bind(beam_energy_distribution,PRNG_source); 



    //----------------------------------------------------------------------------------------------------
    //----------------------------------- Perform some sanity checks -------------------------------------
    //----------------------------------------------------------------------------------------------------

    //Test out the PRNG.
    for(unsigned int i=0; i<10; ++i){
        std::cout << PRNG_source() << " " ;
    }
    std::cout << std::endl;

    //Test out the beam-source energy distribution.
    for(unsigned int i=0; i<10; ++i){
        std::cout << i << " " << beam_energy_distribution( PRNG_source() ) << std::endl ;
        //std::cout << i << " " << new_beam_energy() << std::endl ;
    }
    std::cout << std::endl;


    //Test out the beam position get/sets.
    vec3<double> temp = beam_position();
    std::cout << "Beam location is: " << temp << std::endl;
    vec3<double> new_position( PRNG_source(), PRNG_source(), PRNG_source());
    set_beam_position( new_position );
    temp = beam_position();
    std::cout << "Beam location is: " << temp << std::endl;

    //Test out the new beam-source particle orientation.
    temp = get_new_orientation(PRNG_source(), PRNG_source(), PRNG_source());
    std::cout << "New orientation is: " << temp << std::endl;

    for(unsigned int i=0; i<3; ++i){
        temp = get_new_orientation(PRNG_source(), PRNG_source(), PRNG_source());
        std::cout << "The photon position is: " << temp << std::endl;
    }

    //Test the material bounding box function.
    for(unsigned int i=0; i<6; ++i){
        temp = vec3<double>(10.0*PRNG_source()-5.0, 10.0*PRNG_source()-5.0, 10.0*PRNG_source()-5.0);
        std::cout << "Material at: " << temp << " is " << (int)(which_material(temp)) << std::endl;
    }

    std::cout << (int)(Material::Water) << std::endl;



    //Test the factory functions.
    std::unique_ptr<base_particle> aphoton = photon_factory(1.23, temp, temp);

    if(aphoton == NULL) FUNCERR("Unable to create a particle using factory function");
    std::cout << "Created photon has energy " << aphoton->get_energy() << " which should be 1.23 !" << std::endl;


    aphoton->set_energy(1.32);
    std::cout << "Created photon has energy " << aphoton->get_energy() << " which should be 1.32 !" << std::endl;

    temp = aphoton->get_position3();
    std::cout << "The photon position is: " << temp << std::endl;

    temp.x += 3000.0;
    aphoton->set_position3(temp);
    temp = aphoton->get_position3();
    std::cout << "The photon position is: " << temp << std::endl;





    //----------------------------------------------------------------------------------------------------
    //----------------------------------------- Exit and cleanup -----------------------------------------
    //----------------------------------------------------------------------------------------------------

    //Close all libraries which are open. When we do this, we lose the ability to call the functions we yanked from them.
    for(void *i : open_libraries ){
        close_library(i);
    }

    return 0;
}







