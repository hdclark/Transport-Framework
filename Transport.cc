//Transport.cc - The main entrypoint into the code. See the README file for general information.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <getopt.h>      //Needed for 'getopts' argument parsing.

//#include <random>     //We use this for PRNG's. Not actually needed here?

//#include <dlfcn.h> //Used for dlopen, etc.. Dynamic library loading.
//#include <functional> //Useful for binding ("currying") functions. Used for PRNG.

#include "./Misc.h"  //Various utilities. FUNCINFO, FUNCERR, etc..
#include "./Dynamic_Loading.h" //Routines for dynamically loading shared libraries.
#include "./Typedefs.h"  //Typedefs for the dynamic libraries. These let us slightly more safely cast function pointers.

#include "./MyMath.h" //vec3, etc..
#include "./Structs.h" //vec4, etc.. 

#include "./Constants.h"

bool VERBOSE = false;
const std::string VERSION("0.0.1 - Extreme Beta Version - Use at your own risk!");

//Passed in as arguments.
long int random_seed = 12345677; //Pick a prime
std::vector<void *> open_libraries;  //Keeps track of opened libraries. We need to keep them open until we are done.
unsigned char beam_type; //Which type of particle should come from the beam source. Types are listed in Constants.cc.
double smallest_feature = 0.1;     //The smallest feature in the geometry - useful for transporting particles through a vacuum in a sensible way. This is overwritten by geometry, if it exists in the module!


//----------------------------------------------------------------------------------------------------
//------------------------------------ Dynamically loaded functions ----------------------------------
//----------------------------------------------------------------------------------------------------
//These function types (which, more precisely, define function signatures only) are typedefs which are in Typedefs.h.
FUNCTION_PRNG_source          PRNG_source; //A pseudo-random number generator source/iterator function.
FUNCTION_energy_distribution  beam_energy_distribution; //The beam-source energy distribution. (Not collision distribution.)
//FUNCTION_get_position         beam_position; //Returns the beam source outlet (ie. the source point.)
FUNCTION_set_position         set_beam_position; //Lets us adjust the beam source outlet (ie. the source point.)
FUNCTION_get_orientation      get_new_orientation; //Gets a new orientation unit vector for a particle ejected from the source outlet. (holds the angular distribution of the source).
FUNCTION_random_orientation   get_random_orientation; //Gets a uniformly-distributed orientation unit vector3.
//FUNCTION_geometry_type        which_material; //Returns the char value corresponding to the material at a point in space.
FUNCTION_particle_factory     photon_factory; //Derived class factory function - creates a Photon class instance on the heap.
//FUNCTION_particle_factory     positron_factory;
FUNCTION_particle_factory     electron_factory;

FUNCTION_mean_free_path       mean_free_path; //Given the particle in question, this returns the mean free path for a particluar material.
FUNCTION_which_interaction    which_interaction_water; //Returns the unsigned char indicating the type of interaction a given particle will undergo, given a clamped uniform random.
FUNCTION_mfp_and_which_interaction  water_mfp_and_which_interaction; //Gets both mfp and the interaction but only evaluates the mass attenuation coefficients once.
FUNCTION_particle_sink        particle_sink;  //Used to pass unique_ptrs of instances of base_particle for memory management routines.
FUNCTION_remaining_size       remaining_size; //Returns the number of additional particles which can be handled. (Do not push too hard!)
FUNCTION_get_next_particle    next_particle;  //Returns a unique_ptr to the next active particle in memory.

FUNCTION_scatter_routine      scatter_coherent; //Implements the Coherent scattering routine. 
FUNCTION_scatter_routine      scatter_compton; //Implements the Compton scattering routine. 
FUNCTION_scatter_routine      scatter_photoelectric; //Implements the photoelectric effect ("scatter") routine. 
FUNCTION_scatter_routine      scatter_pair;  //Implements the Pair production scattering routine.
FUNCTION_scatter_routine      scatter_localdump; //Implements the local energy dump ("scatter") routine. 
FUNCTION_scatter_routine      scatter_slowdown; //Implements a CSDA charged particle slow-down, swallows the particle.
FUNCTION_scatter_routine      scatter_none;  //Implements a 'virtual' interaction where nothing happens.
FUNCTION_scatter_routine      scatter_detect; //Implements a detector event - particle has hit a detector.

//Testing - Water/Photons.
FUNCTION_mass_coefficient_X   compton_mass_attenuation;
FUNCTION_mass_coefficient_X   coherent_mass_attenuation;
FUNCTION_mass_coefficient_X   pair_mass_attenuation;
FUNCTION_mass_coefficient_X   photoelectric_mass_attenuation;


//This is used to pass the list of loaded functions around to dynamically loaded modules (and other routines
// so as to make function calling as homogeneous as possible.)
struct Functions  Loaded_Funcs;

//----------------------------------------------------------------------------------------------------
//------------------------------------- Entry into program here --------------------------------------
//----------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]){


    long int numb_of_particles = 0;
    long int numb_of_loop_multiplications = 1;
    long int particles_per_loop = numb_of_particles / numb_of_loop_multiplications;

    std::vector<std::string> libraries;
    //libraries.push_back("/home/hal/Dropbox/Project - Transport/lib_beams.so");
    //libraries.push_back("./lib_photons.so");
    // etc..

    //  INCOMING:      -libraries which need to be loaded,
    //                 -seed value (if desired)
    //                 -verbosity
    //                 -beam type (what we want to come from the beam source)
    //                 -number of particles to use.


    //---------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------ Option parsing -----------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------
    //These are fairly common options. Run the program with -h to see them formatted properly.
    int next_options;
    const char* const short_options    = "hVvp:s:";  //This is the list of short, single-letter options.
                                                     //The : denotes a value passed in with the option.
    //This is the list of long options. Columns:  Name, BOOL: takes_value?, NULL, Map to short options.
    const struct option long_options[] = { { "help",        0, NULL, 'h' },
                                           { "version",     0, NULL, 'V' },
                                           { "verbose",     0, NULL, 'v' },
                                           { "particles",   1, NULL, 'p' },
                                           { "seed",        1, NULL, 's' },
                                           { NULL,          0, NULL, 0   }  };

    do{
        next_options = getopt_long(argc, argv, short_options, long_options, NULL);
        switch(next_options){
            case 'h':
                std::cout << std::endl;
                std::cout << "-- " << argv[0] << " Command line switches: " << std::endl;
                std::cout << "----------------------------------------------------------------------------------------------------------" << std::endl;
                std::cout << "   Short              Long                 Default          Description" << std::endl;
                std::cout << "----------------------------------------------------------------------------------------------------------" << std::endl;
                std::cout << "   -h                 --help                                Display this message and exit." << std::endl;
                std::cout << "   -V                 --version                             Display program version and exit." << std::endl;
                std::cout << "   -v                 --verbose             <false>         Spit out info about what the program is doing." << std::endl;
                std::cout << "   -p < # >           --particles           <none>          Number of particles to use, if appropriate. (Required.)" << std::endl;
                std::cout << "   -s < seed >        --seed                <varies>        Seed value. Takes any input." << std::endl;
                std::cout << std::endl;
                return 0;
                break;

            case 'V':
                FUNCINFO( "Version: " + VERSION );
                return 0;
                break;

            case 'v':
                FUNCINFO("Verbosity enabled");
                VERBOSE = true;
                break;

            case 'p':
                numb_of_particles = stringtoX<long int>( optarg );
                break;

            case 's':
                {
                std::string temp = optarg; 
                random_seed = static_cast<long int>( simple_hash( temp ) );
                } 
                break;

        }
    }while(next_options != -1);

    //Note that other options may be read, but it is safest to add them to the above getargs code properly.

    //From the optind man page:
    //  "If the resulting value of optind is greater than argc, this indicates a missing option-argument,
    //         and getopt() shall return an error indication."
    if(optind < argc)  FUNCERR("Received an option without an argument: \"" << argv[optind] << "\". Run with -h to see help.");


    //---------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------ Option handling ----------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------
    if(numb_of_particles == 0) FUNCERR("Number of particles to run (-p) is required for this simulation.");

    //Sort out which particle cache schedule to use based on the number of particles.
    numb_of_loop_multiplications = 1;
    particles_per_loop = numb_of_particles;
    while( particles_per_loop > 5E4 ){ //Seems like a fairly safe number to use. Maybe there is a better criteria?
        numb_of_loop_multiplications *= 10;
        particles_per_loop = numb_of_particles / numb_of_loop_multiplications;
    }


    libraries.push_back("./lib_photons.so");
    libraries.push_back("./lib_electrons.so");
    libraries.push_back("./lib_positrons.so");
    libraries.push_back("./lib_random_MT.so");
    libraries.push_back("./lib_memory.so");
    libraries.push_back("./lib_coherent.so");
    libraries.push_back("./lib_photoelectric.so");
    libraries.push_back("./lib_compton.so");
    libraries.push_back("./lib_pair.so");
    libraries.push_back("./lib_no_interaction.so");
    libraries.push_back("./lib_localdump.so");
    libraries.push_back("./lib_slowdown.so");
//    libraries.push_back("./lib_water_fitted.so");  //Don't use - haven't updated since adding absorption, transfer,one_minus_g, etc..
    libraries.push_back("./lib_water_csplines.so");
//    libraries.push_back("./lib_water_linear.so");  //Don't use - haven't updated since adding absorption, transfer,one_minus_g, etc..
    libraries.push_back("./lib_logging.so");
    libraries.push_back("./lib_detect.so");          //Not actually needed, but needs to be here for sanity checks. This situation should be handled with a toggle switch. (HAS_DETECTOR?)
    libraries.push_back("./lib_voxel_mapping.so");

/*
//------------ Infinite Water tank setup ---------------
    libraries.push_back("./lib_beam_6MV.so");
    libraries.push_back("./lib_geometry_inf_water.so");
*/


/*
//--------------- Water Slab setup ---------------------
//    libraries.push_back("./lib_beam_1MeV_photons.so");
//    libraries.push_back("./lib_beam_10MeV_photons.so");
    libraries.push_back("./lib_beam_6MV.so");
    libraries.push_back("./lib_geometry_water_slab.so");
*/


//--------------- Water Tank setup ---------------------
    libraries.push_back("./lib_beam_6MV.so");
    libraries.push_back("./lib_geometry_water_tank.so");


/*
//----------------- CT geometry ------------------------
    libraries.push_back("./lib_beam_xray_N7599.so");
    libraries.push_back("./lib_geometry_CT_imager.so");
*/


    FUNCINFO("Proceeding with random seed " << random_seed ); 
    FUNCINFO("Proceeding with " << numb_of_particles << " particles (using " << particles_per_loop << " per loop.)");

    //---------------------------------------------------------------------------------------------------------------------
    //--------------------------------------------- Shared Library Loading ------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------

    std::string Beam_ID;  //6MV, 1MeV, 10MeV, etc.. Useful for automatically switching on logging routines.

    //Load in all libraries in the libraries string vector.
    //Program will simply halt if the file is not found in any of the usual places!
    for(std::string library_fullpath : libraries){
        void *loaded_library = load_library(library_fullpath);
    
        if(loaded_library != NULL){
            open_libraries.push_back( loaded_library );
            std::string FileType;
            std::string ParticleType;
            std::string MediumType;
            std::string InteractionType;

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
        
            //Load the medium type identifier string.
            if(check_for_item_in_library( loaded_library, "MEDIUM_TYPE")){
                MediumType = *reinterpret_cast<std::string *>(load_item_from_library(loaded_library, "MEDIUM_TYPE"));
            }

             //Load the interaction type identifier string.
            if(check_for_item_in_library( loaded_library, "INTERACTION_TYPE")){
                InteractionType = *reinterpret_cast<std::string *>(load_item_from_library(loaded_library, "INTERACTION_TYPE"));
            }

            //------------------------------- Set up the random number generator -------------------------------
            if(FileType == "PRNG"){
                //Seed the generator. It will be randomly (non-reproduceably) seeded otherwise.
                if(check_for_item_in_library( loaded_library, "init_explicit_seed")){
                    FUNCTION_init_explicit_seed PRNG_seed = reinterpret_cast<FUNCTION_init_explicit_seed>(load_item_from_library(loaded_library, "init_explicit_seed") );
                    PRNG_seed(random_seed);
                }
   
                //Grab the source/iterator function.
                if(check_for_item_in_library( loaded_library, "source")){
                    PRNG_source = reinterpret_cast<FUNCTION_PRNG_source>(load_item_from_library(loaded_library, "source") );
                    Loaded_Funcs.PRNG_source = reinterpret_cast<FUNCTION_PRNG_source>(load_item_from_library(loaded_library, "source") );
                }

                //Get the random orientation function. Note that this is separate from the beam orientation, which may or
                // may not be random (and has a different signature!)
                if(check_for_item_in_library( loaded_library, "get_random_orientation")){
                    Loaded_Funcs.get_random_orientation = reinterpret_cast<FUNCTION_random_orientation>(load_item_from_library(loaded_library, "get_random_orientation") );
                }

            //----------------------------------- Set up memory management -------------------------------------
            }else if(FileType == "MEMORY"){
                //Grab the sink (aka "pass-to-memory-scheme") function.
                if(check_for_item_in_library( loaded_library, "particle_sink")){
                    particle_sink = reinterpret_cast<FUNCTION_particle_sink>(load_item_from_library(loaded_library, "particle_sink") );
                    Loaded_Funcs.particle_sink = reinterpret_cast<FUNCTION_particle_sink>(load_item_from_library(loaded_library, "particle_sink") );
                } 

                //Grab the free-space indicator function - returns the number of *particles* which can be stored (approximately.)
                if(check_for_item_in_library( loaded_library, "how_much_more_room")){
                    remaining_size = reinterpret_cast<FUNCTION_remaining_size>(load_item_from_library(loaded_library, "how_much_more_room") );
                }

                //Grab the next particle iterator (source) function.
                if(check_for_item_in_library( loaded_library, "get_next_particle")){
                    next_particle = reinterpret_cast<FUNCTION_get_next_particle>(load_item_from_library(loaded_library, "get_next_particle") );
                }


            //---------------------------------- Set up the beam geometry --------------------------------------
            }else if(FileType == "BEAM"){
                //Grab the energy distribution function.
                if(check_for_item_in_library( loaded_library, "energy_distribution")){
                    beam_energy_distribution = reinterpret_cast<FUNCTION_energy_distribution>(load_item_from_library(loaded_library, "energy_distribution") );
                }

                //Grab the Beam ID.
                if(check_for_item_in_library( loaded_library, "BEAM_TYPE")){
                    Beam_ID = *reinterpret_cast<std::string *>(load_item_from_library(loaded_library, "BEAM_TYPE"));
                }

            //------------------------------ Set up the geometry bounding boxes --------------------------------
            }else if(FileType == "GEOMETRY"){
                //Grab the material bounding box function.
                if(check_for_item_in_library( loaded_library, "geometry_type")){
                    Loaded_Funcs.which_material = reinterpret_cast<FUNCTION_geometry_type>(load_item_from_library(loaded_library, "geometry_type") );
                }

                //Update the smallest_feature to that of the geometry. This will help set the length scale for vacuum transport.
                if(check_for_item_in_library( loaded_library, "SMALLEST_FEATURE")){
                    smallest_feature = *reinterpret_cast<double *>(load_item_from_library(loaded_library, "SMALLEST_FEATURE"));
                }

                //Grab the beam position get/set functions.
                if(check_for_item_in_library( loaded_library, "get_position")){
                    Loaded_Funcs.beam_position = reinterpret_cast<FUNCTION_get_position>(load_item_from_library(loaded_library, "get_position"));
                }
                if(check_for_item_in_library( loaded_library, "set_position")){
                    set_beam_position = reinterpret_cast<FUNCTION_set_position>(load_item_from_library(loaded_library, "set_position"));
                }

                //Grab the beam orientation distribution function.
                if(check_for_item_in_library( loaded_library, "get_orientation")){
                    get_new_orientation = reinterpret_cast<FUNCTION_get_orientation>(load_item_from_library(loaded_library, "get_orientation"));
                }


            //------------------------------------- Set up the Materials ---------------------------------------
            }else if(MediumType == "WATER"){
                //Grab the mass coefficient interpolation routines (for testing!) 
                if(check_for_item_in_library( loaded_library, "photon_mass_coefficient_coherent")){
                    coherent_mass_attenuation = reinterpret_cast<FUNCTION_mass_coefficient_X>(load_item_from_library(loaded_library, "photon_mass_coefficient_coherent") );
                }
                if(check_for_item_in_library( loaded_library, "photon_mass_coefficient_compton")){
                    compton_mass_attenuation  = reinterpret_cast<FUNCTION_mass_coefficient_X>(load_item_from_library(loaded_library, "photon_mass_coefficient_compton") );
                }
                if(check_for_item_in_library( loaded_library, "photon_mass_coefficient_pair_triplet")){
                    pair_mass_attenuation     = reinterpret_cast<FUNCTION_mass_coefficient_X>(load_item_from_library(loaded_library, "photon_mass_coefficient_pair_triplet") );
                }
                if(check_for_item_in_library( loaded_library, "photon_mass_coefficient_photoelectric")){
                    photoelectric_mass_attenuation = reinterpret_cast<FUNCTION_mass_coefficient_X>(load_item_from_library(loaded_library, "photon_mass_coefficient_photoelectric") );
                }

                //Grab the total, absorption, transfer mass attenuation coefficients. These are used for computing kerma and dose.
                if(check_for_item_in_library( loaded_library, "photon_mass_coefficient_total")){
                    Loaded_Funcs.photon_mass_coefficient_total = reinterpret_cast<FUNCTION_mass_coefficient_X>(load_item_from_library(loaded_library, "photon_mass_coefficient_total") );
                }
                if(check_for_item_in_library( loaded_library, "photon_mass_coefficient_transfer")){
                    Loaded_Funcs.photon_mass_coefficient_transfer = reinterpret_cast<FUNCTION_mass_coefficient_X>(load_item_from_library(loaded_library, "photon_mass_coefficient_transfer") );
                }
                if(check_for_item_in_library( loaded_library, "photon_mass_coefficient_absorption")){
                    Loaded_Funcs.photon_mass_coefficient_absorption = reinterpret_cast<FUNCTION_mass_coefficient_X>(load_item_from_library(loaded_library, "photon_mass_coefficient_absorption") );
                }

                //Grab the average energy transferred, absorbed functions.
                if(check_for_item_in_library( loaded_library, "photon_average_energy_absorbed")){
                    Loaded_Funcs.photon_average_energy_absorbed = reinterpret_cast<FUNCTION_average_energy_X>(load_item_from_library(loaded_library, "photon_average_energy_absorbed") );
                }
                if(check_for_item_in_library( loaded_library, "photon_average_energy_transferred")){
                    Loaded_Funcs.photon_average_energy_transferred = reinterpret_cast<FUNCTION_average_energy_X>(load_item_from_library(loaded_library, "photon_average_energy_transferred") );
                }

                //Grab the mean-free-path function.
                if(check_for_item_in_library( loaded_library, "mean_free_path")){
                    mean_free_path = reinterpret_cast<FUNCTION_mean_free_path>(load_item_from_library(loaded_library, "mean_free_path") );
                }

                //Grab the particle interaction distribution.
                if(check_for_item_in_library( loaded_library, "which_interaction")){
                    which_interaction_water = reinterpret_cast<FUNCTION_which_interaction>(load_item_from_library(loaded_library, "which_interaction") );
                }

                //Grab the mean-free-path AND particle interaction distribution function.
                if(check_for_item_in_library( loaded_library, "mean_free_path_and_which_interaction")){
                    water_mfp_and_which_interaction = reinterpret_cast<FUNCTION_mfp_and_which_interaction>(load_item_from_library(loaded_library, "mean_free_path_and_which_interaction") );
                }


            //--------------------------------- Set up the photon functions ------------------------------------
            }else if(ParticleType == "PHOTON"){
                //Grab the Photon particle class factory function. (It polymorphs the base_particle class!)
                if(check_for_item_in_library( loaded_library, "particle_factory")){
                    photon_factory = reinterpret_cast<FUNCTION_particle_factory>(load_item_from_library(loaded_library, "particle_factory") );
                    Loaded_Funcs.photon_factory = reinterpret_cast<FUNCTION_particle_factory>(load_item_from_library(loaded_library, "particle_factory") );
                }


            //-------------------------------- Set up the electron functions -----------------------------------
            }else if(ParticleType == "ELECTRON"){
                //Grab the electron particle class factory function. (It polymorphs the base_particle class!)
                if(check_for_item_in_library( loaded_library, "particle_factory")){
                    electron_factory = reinterpret_cast<FUNCTION_particle_factory>(load_item_from_library(loaded_library, "particle_factory") );
                    Loaded_Funcs.electron_factory = reinterpret_cast<FUNCTION_particle_factory>(load_item_from_library(loaded_library, "particle_factory") );
                }


            //-------------------------------- Set up the positron functions -----------------------------------
            }else if(ParticleType == "POSITRON"){
                //Grab the positron particle class factory function. (It polymorphs the base_particle class!)
                if(check_for_item_in_library( loaded_library, "particle_factory")){
                    Loaded_Funcs.positron_factory = reinterpret_cast<FUNCTION_particle_factory>(load_item_from_library(loaded_library, "particle_factory") );
                }
            

            //---------------------------- Set up the Coherent scatter functions --------------------------------
            }else if(InteractionType == "COHERENT"){
                //Grab the coherent scattering routine.
                if(check_for_item_in_library( loaded_library, "scatter")){
                    scatter_coherent = reinterpret_cast<FUNCTION_scatter_routine>(load_item_from_library(loaded_library, "scatter") );
                }

            //---------------------------- Set up the SlowDown scatter functions --------------------------------
            }else if(InteractionType == "SLOWDOWN"){
                //Grab the coherent scattering routine.
                if(check_for_item_in_library( loaded_library, "scatter")){
                    scatter_slowdown = reinterpret_cast<FUNCTION_scatter_routine>(load_item_from_library(loaded_library, "scatter") );
                }


            //---------------------------- Set up the Photoelectric effect functions --------------------------------
            }else if(InteractionType == "PHOTOELECTRIC"){
                //Grab the photoelectric effect "scattering" routine.
                if(check_for_item_in_library( loaded_library, "scatter")){
                    scatter_photoelectric = reinterpret_cast<FUNCTION_scatter_routine>(load_item_from_library(loaded_library, "scatter") );
                }

            //---------------------------- Set up the Compton scatter functions --------------------------------
            }else if(InteractionType == "COMPTON"){
                //Grab the Compton scattering routine.
                if(check_for_item_in_library( loaded_library, "scatter")){
                    scatter_compton = reinterpret_cast<FUNCTION_scatter_routine>(load_item_from_library(loaded_library, "scatter") );
                }


            //---------------------------- Set up the Pair production scatter functions --------------------------------
            }else if(InteractionType == "PAIR"){
                //Grab the pair-production "scattering" routine.
                if(check_for_item_in_library( loaded_library, "scatter")){
                    scatter_pair = reinterpret_cast<FUNCTION_scatter_routine>(load_item_from_library(loaded_library, "scatter") );
                }


            //---------------------------- Set up the no-interaction scatter functions --------------------------------
            }else if(InteractionType == "NONE"){
                //Grab the no-interaction "scattering" routine.
                if(check_for_item_in_library( loaded_library, "scatter")){
                    scatter_none = reinterpret_cast<FUNCTION_scatter_routine>(load_item_from_library(loaded_library, "scatter") );
                }

            //---------------------------- Set up the Localdump scatter functions --------------------------------
            }else if(InteractionType == "LOCALDUMP"){
                //Grab the coherent scattering routine.
                if(check_for_item_in_library( loaded_library, "scatter")){
                    scatter_localdump = reinterpret_cast<FUNCTION_scatter_routine>(load_item_from_library(loaded_library, "scatter") );
                }

            //---------------------------- Set up the Detection scatter functions --------------------------------
            }else if(InteractionType == "DETECTION"){
                //Grab the detection scattering routine.
                if(check_for_item_in_library( loaded_library, "scatter")){
                    scatter_detect = reinterpret_cast<FUNCTION_scatter_routine>(load_item_from_library(loaded_library, "scatter") );
                }


            //---------------------------- Set up the logging routines --------------------------------
            }else if(FileType == "LOGGING"){
                //Grab the particle graveyard routine.
                if(check_for_item_in_library( loaded_library, "particle_graveyard")){
                    Loaded_Funcs.particle_graveyard = reinterpret_cast<FUNCTION_particle_graveyard>(load_item_from_library(loaded_library, "particle_graveyard") );
                }

                //Grab the generic logging function.
                if(check_for_item_in_library( loaded_library, "logging_generic")){
                    Loaded_Funcs.generic_logging = reinterpret_cast<FUNCTION_generic_logging>(load_item_from_library(loaded_library, "logging_generic") );
                }


            //---------------------------- Set up the voxel routines --------------------------------
            }else if(FileType == "VOXEL"){
                //Grab the CSDA slowdown routine.
                if(check_for_item_in_library( loaded_library, "accumulate_slowdown")){
                    Loaded_Funcs.voxel_accumulation = reinterpret_cast<FUNCTION_accumulate_slowdown>(load_item_from_library(loaded_library, "accumulate_slowdown") );
                }

                //Grab the voxel localdump routine.
                if(check_for_item_in_library( loaded_library, "voxel_localdump")){
                    Loaded_Funcs.voxel_localdump = reinterpret_cast<FUNCTION_voxel_localdump>(load_item_from_library(loaded_library, "voxel_localdump") );
                }

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
//          || (get_random_orientation == NULL )
          || (Loaded_Funcs.beam_position == NULL )
          || (set_beam_position == NULL )
          || (Loaded_Funcs.which_material == NULL )
          || (photon_factory == NULL )
          || (electron_factory == NULL )
          || (mean_free_path == NULL )
          || (particle_sink == NULL )
          || (next_particle == NULL )
          || (Loaded_Funcs.get_random_orientation == NULL )
          || (Loaded_Funcs.PRNG_source == NULL )
          || (Loaded_Funcs.particle_sink == NULL )
          || (Loaded_Funcs.photon_factory == NULL )
          || (Loaded_Funcs.electron_factory == NULL )
          || (Loaded_Funcs.positron_factory == NULL )
          || (Loaded_Funcs.particle_graveyard == NULL )
          || (scatter_coherent == NULL )
          || (scatter_photoelectric == NULL )
          || (scatter_compton == NULL )
          || (scatter_pair == NULL )
          || (scatter_localdump == NULL )
          || (scatter_slowdown == NULL )
          || (scatter_detect == NULL )
          || (scatter_none == NULL )
          || (which_interaction_water == NULL )
          || (Loaded_Funcs.voxel_accumulation == NULL )
          || (Loaded_Funcs.voxel_localdump == NULL )

          || (Loaded_Funcs.photon_mass_coefficient_total == NULL )
          || (Loaded_Funcs.photon_mass_coefficient_absorption == NULL )
          || (Loaded_Funcs.photon_mass_coefficient_transfer == NULL )
          || (Loaded_Funcs.photon_average_energy_absorbed == NULL )
          || (Loaded_Funcs.photon_average_energy_transferred == NULL )          
          || (coherent_mass_attenuation == NULL )
          || (compton_mass_attenuation == NULL )
          || (photoelectric_mass_attenuation == NULL )
          || (pair_mass_attenuation == NULL )
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
    if(LoggingQuantities::MassAttenuationCoefficients){
       //Print out the mass attenuation coefficients for verification.
       Loaded_Funcs.generic_logging("Mass_Attenuation_Coefficients") << "# Mass attenuation coefficients. " << std::endl;
       Loaded_Funcs.generic_logging("Mass_Attenuation_Coefficients") << "#   Energy   Coherent   Compton   Photoelectric   Pair   Total " << std::endl;
       for(double E=0.001001; E<=50.0; E *= 1.01){ //(Output nicely for a log scale.)
           double comp = compton_mass_attenuation(E);
           double chnt = coherent_mass_attenuation(E);
           double phot = photoelectric_mass_attenuation(E);
           double pair = pair_mass_attenuation(E);
           double tot = comp + chnt + phot + pair;
           Loaded_Funcs.generic_logging("Mass_Attenuation_Coefficients") << E << " " << chnt << " " << comp << " " << phot << " " << pair << " " << tot << std::endl;
       }
    }

    if(LoggingQuantities::MFPWaterSampling){
        //Generates a large number of mfp's to check the average mfp spectrum *_*_*_*_*FOR WATER*_*_*_*_*_*
        Loaded_Funcs.generic_logging("Sampled_MFP") << "# Sampled MFP's in WATER. Useful for constructing geometry which might be affected by the wrong choice of component thickness." << std::endl;
        Loaded_Funcs.generic_logging("Sampled_MFP") << "# energy   mfp " << std::endl;
        {
          const double E_max = 10.00;
          const double E_min = 0.01;
          std::unique_ptr<base_particle> a_photon = photon_factory(E_max, vec3<double>(0.0,0.0,0.0), vec3<double>(1.0,1.0,1.0));
          double dl;
          unsigned char which_interaction; 
          for(size_t i=0; i<100000; ++i){
              a_photon->set_energy( PRNG_source() * (E_max-E_min) + E_min );
              water_mfp_and_which_interaction( a_photon.get(), PRNG_source(), PRNG_source(), which_interaction, dl);
              Loaded_Funcs.generic_logging("Sampled_MFP") << a_photon->get_energy() << " " << dl << std::endl;
          }
        }
    }
    
    if(LoggingQuantities::BeamSpectrumSampling){
        //Generates a large number of photons to check beam energy spectrum.
        Loaded_Funcs.generic_logging("Sampled_Spectrum") << "# Sampled spectrum. Useful for direct (random) measurement of spectrum output from source." << std::endl;
        Loaded_Funcs.generic_logging("Sampled_Spectrum") << "# Energy    <arbitrary number> " << std::endl;
        for(size_t i=0; i<100000; ++i){
            Loaded_Funcs.generic_logging("Sampled_Spectrum") << beam_energy_distribution( Loaded_Funcs ) << " " << i << std::endl;
        }
    }

    if(LoggingQuantities::CTGeometry){
        //Outputs the material type for a randomly-sampled geometry test for the CT_scanner geometry.
        for(size_t i=0; i<100000; ++i){
            const double x = 2.0*20.0*(PRNG_source()-0.5);
            const double z = 2.0*20.0*(PRNG_source()-0.5);
            const double y = 0;
            std::cout << x << " " << y << " " << z << " " << (int)( Loaded_Funcs.which_material(vec3<double>(x,y,z)) ) << std::endl;
        }
    }
    
    if(LoggingQuantities::CTDetectorGeometry){   
        //Outputs the material type for a randomly-sampled geometry test for the CT_scanner geometry. Close up zoom on the comb filter.
        Loaded_Funcs.generic_logging("Sampled_Comb") << "# This is a close-up of the detector combs. It should be punctuated and literally look like a comb." << std::endl;
        Loaded_Funcs.generic_logging("Sampled_Comb") << "#  x  y  z   material " << std::endl;
        for(size_t i=0; i<10000; ++i){
            const double x = 5.0*(2.0*PRNG_source()-1.0);
            const double z = 0.5*(2.0*PRNG_source()-1.0) + 13.5;
            const double y = 0;
            Loaded_Funcs.generic_logging("Sampled_Comb") << x << " " << y << " " << z << " " << (int)( Loaded_Funcs.which_material(vec3<double>(x,y,z)) ) << std::endl;
        }
    }
       
    if(LoggingQuantities::CTPathMaterial){ 
        //Outputs the material type for a path along the beam axis for the CT_scanner geometry.
        for(double z = 20.0; z>-20.0; z-=0.1){
            std::cout << 0.0 << " " << 0.0 << " " << z << " " << (int)( Loaded_Funcs.which_material(vec3<double>(0.0,0.0,z)) ) << std::endl;
        }
    }


    //----------------------------------------------------------------------------------------------------
    //------------------------------------- Perform the simulation ---------------------------------------
    //----------------------------------------------------------------------------------------------------

    for(long int loop_multiplier=0; loop_multiplier<numb_of_loop_multiplications; ++loop_multiplier){ //This is a simple loop used to repeatedly fill the particle cache with new particles. This is used to reduce memory usage.

        //First, we create a bunch of photons at the beam position with a distribution of energy and orientation
        // as indicated by the beam arrangement.
        for(long int i=0; i<particles_per_loop; ++i){
            const double E   =  beam_energy_distribution( Loaded_Funcs );
            vec3<double> pos =  Loaded_Funcs.beam_position( Loaded_Funcs );
            vec3<double> mom =  get_new_orientation(PRNG_source(),PRNG_source(),PRNG_source()) * E;
    
            std::unique_ptr<base_particle> temp = photon_factory(E, pos, mom);
            temp->Interactions.push_back( an_interaction(Interactiontype::Creation, Material::Beam, E, pos));
            particle_sink( std::move( temp ) );
        }
    
    
        //Now we cycle through the remaining particles until they have all deposited their energy somewhere.
        std::unique_ptr<base_particle> current_particle = next_particle();
    //    vec3<double> pos_copy;  //If needed, to try speed up vec3<double> calculations.
    //    double step_factor;     //Used for variable-length ray casting through vacuum.
        while(current_particle != nullptr){

            //Move the particle this distance in the direction of the momentum vector.
            vec3<double> pos = current_particle->get_position3();
            vec3<double> dir = (current_particle->get_relativistic_three_momentum3()).unit(); //This is the unit vector in the direction of travel. Need to have a .unit() here to homogeneously treat particles.
    
            double dl;
            unsigned char material = Loaded_Funcs.which_material(pos); //The *current* particle position, so we know which mfp to use.
            unsigned char which_interaction;
    
            //Determine the distance the photon will travel prior to next interaction and also which interaction type to perform.
            //
            //We can override what the material told us if the particle satisfies our sepuku criteria (if they exist and are turned on.)
    
            //Interaction-number discriminating conditions.
            if((INTERACTION_COUNT_MAX_CULL != 0) && (current_particle->Interactions.size() > INTERACTION_COUNT_MAX_CULL)){
                dl = 0.0;
                which_interaction = Interactiontype::Disappear;
    
            //Sepuku-discriminating conditions.
            }else if((ELECTRON_SEPUKU_LOCALDUMP == true) && (current_particle->get_type() == Particletype::Electron) && (current_particle->get_energy() <= ELECTRON_SEPUKU_ENERGY_THRESHOLD)){
                dl = 0.0;
                which_interaction = Interactiontype::LocalDump;
    
            }else if((POSITRON_SEPUKU_LOCALDUMP == true) && (current_particle->get_type() == Particletype::Positron) && (current_particle->get_energy() <= POSITRON_SEPUKU_ENERGY_THRESHOLD)){
                dl = 0.0;
                which_interaction = Interactiontype::LocalDump;
    
            //Material-discriminating conditions.
            }else if(material == Material::Beam){
                FUNCERR("Particle detected in region of material 'beam'. This is not a geometrically accessible material. Please verify the geometry module and this code.");
    
            }else if(material == Material::Black){
                dl = 0.0;
                which_interaction = Interactiontype::Disappear; //Particle will be completely removed and not logged.
    
            }else if(material == Material::Vacuum){
                dl = Loaded_Funcs.PRNG_source() * smallest_feature;  //This should look ahead to a non-vacuum region and set the mfp to that distance to speed up transport to interacting media..
    
    /*   //Test this scheme more when I have a better data flow :/
                dl = 0.0;  
                step_factor = 1.0;
                pos_copy = pos; 
    
                do {
                    if(step_factor <= 7.5){ //7.5 times the smallest feature.    ...this is probably a bad idea...
                        step_factor += 0.47501*step_factor;   //Used to try reduce the total number of times we have to evaluate the geometry, whilst not overshooting the next material.
                    }    
               
                    pos_copy += dir*smallest_feature * step_factor;
                    dl       += smallest_feature * step_factor;
                   
                }while(Loaded_Funcs.which_material(pos_copy) != Material::Vacuum);
    
                //Now back off one step so we remain in the material.
                dl -= smallest_feature * step_factor;
    */
    
                which_interaction = Interactiontype::None;
    
            }else if(material == Material::Air){
                FUNCERR("This material type is not yet defined. Fix me first!");
    
            }else if(material == Material::Water){
                water_mfp_and_which_interaction( current_particle.get(), PRNG_source(), PRNG_source(), which_interaction, dl);
    
            }else if(material == Material::Detector){
                dl = 0.0;
                which_interaction = Interactiontype::Detect; //Particle will be logged. in *typical* detector setups, we *only* log particles at the detector.
    
            }else{
                FUNCERR("Particle is in a region of unknown material. Verify the geometry module and this code.");
            }
    
            pos +=  dir*dl;
            current_particle->set_position3( pos );

//std::cout << "Transport: dl, dir, pos = " << dl << " " << dir << " " << pos << std::endl;    
    
            //Now I need to decide if the material has changed whilst moving through the MFP distance. If it did, maybe we should re-choose the interaction type.
            //Maybe it is too much of a hassle/too costly to do so?
    /*
            const unsigned char material2 = Loaded_Funcs.which_material(pos);
            if(material != material2){
                material = material2;
    
    
            }
    */
    
            // ------  Logging -------------------------
            //Percentage-Depth Kerma for delta-function beam sources.
            if( (LoggingQuantities::PDK_1MEV || LoggingQuantities::PDK_10MEV)
                && ((Beam_ID == "1MEV") || (Beam_ID == "10MEV"))
                && (current_particle->get_type() == Particletype::Photon) 
                && (current_particle->Interactions.size() == 1) 
                && (which_interaction != Interactiontype::None)
                && (which_interaction != Interactiontype::Disappear)
                && (which_interaction != Interactiontype::LocalDump)
                && (which_interaction != Interactiontype::Detect)
              ){
                //If the photon is originally of energy 1 MeV or 10 MeV, we use its energy to compute the average energy transferred
                // and the total mass attenuation coefficient. This will give us Kerma. 
                //
                //BUT, because we are interested in specific beam energies, and only interested in on-beam-axis (non-scattered) photons,
                // we can simply output the depth the photon is at
    
                if( (current_particle->Interactions[0].energy == 1.0) && (LoggingQuantities::PDK_1MEV) ){
                    Loaded_Funcs.generic_logging("PD_Kerma_1MeV") << pos.distance( current_particle->Interactions[0].position ) << std::endl;
    
                }else if( (current_particle->Interactions[0].energy == 10.0) && (LoggingQuantities::PDK_10MEV) ){
                    Loaded_Funcs.generic_logging("PD_Kerma_10MeV") << pos.distance( current_particle->Interactions[0].position ) << std::endl;
                }
     
            //Percentage-Depth Dose for 6MV beam.
            }else if( (LoggingQuantities::PDD_6MV)
                && (Beam_ID == "6MV") 
                && (current_particle->get_type() == Particletype::Photon)
                && (current_particle->Interactions.size() == 1) 
                && (which_interaction != Interactiontype::None)
                && (which_interaction != Interactiontype::Disappear)
                && (which_interaction != Interactiontype::LocalDump)
                && (which_interaction != Interactiontype::Detect)
              ){
    
                //The kerma and/or dose for a non-delta function spectrum is more difficult to compute. We do it in two steps. First, we
                // output three pieces of data: distance, photon energy, and a part of the kerma integral. We will piece the rest together
                // with a script to bin two dimensions (distance and energy) and then numerically integrate the bins over energy. This 
                // will leave us with binned data along the distance dimension. We can scale it to the maximum bin to get the percent-depth
                // kerma and/or dose.
                const double E = current_particle->get_energy();
                //Dose (use <Eabs>)
                Loaded_Funcs.generic_logging("PD_Dose_6MV") << pos.distance( current_particle->Interactions[0].position ) << " " << E << " " \
                                      << Loaded_Funcs.photon_mass_coefficient_total(E)*Loaded_Funcs.photon_average_energy_absorbed(E) << std::endl;
     
                //Kerma (use <Etrans>)
                //Loaded_Funcs.generic_logging("PD_Dose_6MV") << pos.distance( current_particle->Interactions[0].position ) << " " << E << " " \
                //                      << Loaded_Funcs.photon_mass_coefficient_total(E)*Loaded_Funcs.photon_average_energy_absorbed(E) << std::endl;
            
    
            }
    
            //if(VERBOSE)  FUNCINFO("Newly moved particle has E, position, momentum, and type: " << current_particle->get_energy() << " " << current_particle->get_position3() << " " << current_particle->get_relativistic_three_momentum3() << " " << (int)(current_particle->get_type()) );
    
            //Mark the particle as having undergone the interaction it is about to undergo (so that we do not have to stick this in each interaction library..)
            if( track_interactions == true ){
                current_particle->Interactions.push_back( an_interaction( which_interaction, material, current_particle->get_energy(), current_particle->get_position3() ) );     
            }
    
            //Send the particle into the interaction function. It takes ownership and will probably destroy it,
            // so do not use the reference after this point.
            if( which_interaction == Interactiontype::Compton ){
                scatter_compton( std::move( current_particle ), Loaded_Funcs );
    
            }else if( which_interaction == Interactiontype::Coherent ){
                scatter_coherent( std::move( current_particle ), Loaded_Funcs );
    
            }else if( which_interaction == Interactiontype::Photoelectric ){
                scatter_photoelectric( std::move( current_particle ), Loaded_Funcs );
    
            }else if( which_interaction == Interactiontype::Pair ){
                scatter_pair( std::move( current_particle ), Loaded_Funcs );
    
            }else if( which_interaction == Interactiontype::LocalDump ){
                scatter_localdump( std::move( current_particle ), Loaded_Funcs );
    
            }else if( which_interaction == Interactiontype::SlowDown ){
                scatter_slowdown( std::move( current_particle ), Loaded_Funcs );
    
            }else if( which_interaction == Interactiontype::Detect ){
                scatter_detect( std::move( current_particle ), Loaded_Funcs );
    
            }else if( which_interaction == Interactiontype::None ){
                scatter_none( std::move( current_particle ), Loaded_Funcs ); //This is here in case we want to log or cull particles.
    
            }else if( which_interaction == Interactiontype::Disappear ){
                //Particle will simply disappear right now. We do not log this - disappearance means we don't care about it.
    
            }else{
                FUNCERR("Instructed to perform an interaction (" << (int)(which_interaction) << ") which is unknown!");
            }
    
    
            //Grab the next available active particle.
            current_particle = next_particle();
        }
    
    }

    //----------------------------------------------------------------------------------------------------
    //----------------------------------------- Exit and cleanup -----------------------------------------
    //----------------------------------------------------------------------------------------------------

    //Close all libraries which are open. When we do this, we lose the ability to call the functions we yanked from them.
    for(void *i : open_libraries ){
        close_library(i);
    }

    return 0;
}








