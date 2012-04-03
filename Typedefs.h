//Typedefs.h - Collection of function pointer typedefs corresponding to the prototype module functions.

#ifndef TYPEDEFS_H_TRANSPORT_
#define TYPEDEFS_H_TRANSPORT_

#include <memory>
#include "./MyMath.h"
#include "./Constants.h"

#include <string>
#include <fstream>

//#include "./Structs.h"    // <---- Forward declaration is better.
class base_particle;


//-------------------------------------------------------------------------------------------------------
//--------------------------------------- Catch-all functions -------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: void toggle_verbosity(bool)
typedef void (*FUNCTION_toggle_verbosity_t)(bool);


//-------------------------------------------------------------------------------------------------------
//----------------------------------------------- PRNG's ------------------------------------------------
//-------------------------------------------------------------------------------------------------------

//Used for: bool init_explicit_seed(long int)
typedef bool (*FUNCTION_init_explicit_seed)(long int);

//Used for: double source(void)
typedef double (*FUNCTION_PRNG_source)(void);

//Used for: vec3<double> get_random_orientation(void);
typedef vec3<double> (*FUNCTION_random_orientation)(void);


//-------------------------------------------------------------------------------------------------------
//----------------------------------------------- Beams -------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: void set_position(const vec3<double> &in);
typedef void (*FUNCTION_set_position)(const vec3<double> &);

//Used for: vec3<double> get_position(const struct Functions &);
typedef vec3<double> (*FUNCTION_get_position)(const struct Functions &);

//Used for: vec3<double> get_orientation(const double &ina, const double &inb, const double &inc);
typedef vec3<double> (*FUNCTION_get_orientation)(const double &ina, const double &inb, const double &inc);

//Used for: double energy_distribution(const double &in)
typedef double (*FUNCTION_energy_distribution)(const struct Functions &);


//-------------------------------------------------------------------------------------------------------
//--------------------------------------------- Geometry ------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: unsigned char geometry_type(const vec3<double> &in);
typedef unsigned char (*FUNCTION_geometry_type)(const vec3<double> &in);


//-------------------------------------------------------------------------------------------------------
//---------------------------------------------- Memory -------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: void particle_sink( std::unique_ptr<base_particle> in );
typedef void (*FUNCTION_particle_sink)(std::unique_ptr<base_particle>);

//Used for: size_t how_much_more_room( void );
typedef size_t (*FUNCTION_remaining_size)(void);

//Used for: std::unique_ptr<base_particle> get_next_particle(void);
typedef std::unique_ptr<base_particle> (*FUNCTION_get_next_particle)(void);


//-------------------------------------------------------------------------------------------------------
//---------------------------------------------- Photons ------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: std::unique_ptr<base_particle> particle_factory(const double &energy_in, const vec3<double> &position, const vec3<double> &orientation) (photons)
//Used for: std::unique_ptr<base_particle> particle_factory(const double &energy_in, const vec3<double> &position, const vec3<double> &orientation) (electrons)
typedef std::unique_ptr<base_particle> (*FUNCTION_particle_factory)(const double &, const vec3<double> &, const vec3<double> &);


//-------------------------------------------------------------------------------------------------------
//----------------------------------------------- Water -------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: double photon_mass_coefficient_pair_triplet(const double &E);
//Used for: double photon_mass_coefficient_photoelectric(const double &E);
//Used for: double photon_mass_coefficient_compton(const double &E);
//Used for: double photon_mass_coefficient_coherent(const double &E);
//Used for: double photon_mass_coefficient_total(const double &E);
//Used for: double photon_mass_coefficient_transfer(const double &E);
typedef double (*FUNCTION_mass_coefficient_X)(const double &);

//Used for: double photon_average_energy_absorbed(const double &E );
//Used for: double photon_average_energy_transferred(const double &E );
typedef double (*FUNCTION_average_energy_X)(const double &);


//Used for: double mean_free_path( base_particle *in, const double &clamped );
typedef double (*FUNCTION_mean_free_path)( base_particle *, const double &);

//Used for: unsigned char which_interaction( base_particle *in, const double &clamped );
typedef unsigned char (*FUNCTION_which_interaction)( base_particle *, const double &);

//Used for: void mean_free_path_and_which_interaction( base_particle *in, const double &clamped1, const double &clamped2, unsigned char &which, double &mfp);
typedef void (*FUNCTION_mfp_and_which_interaction)( base_particle *, const double &, const double &, unsigned char &, double &);

//-------------------------------------------------------------------------------------------------------
//--------------------------------------- Scattering Routines -------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: void scatter(std::unique_ptr<base_particle> A, const struct Functions &Loaded_Functions);   (Coherent Scattering)
//Used for: void scatter(std::unique_ptr<base_particle> A, const struct Functions &Loaded_Functions);   (Photoelectric Effect)
//Used for: void scatter(std::unique_ptr<base_particle> A, const struct Functions &Loaded_Functions);   (Compton Scattering)
//Used for: void scatter(std::unique_ptr<base_particle> A, const struct Functions &Loaded_Functions);   (Pair-production)
//Used for: void scatter(std::unique_ptr<base_particle> A, const struct Functions &Loaded_Functions);   ('Local dump' scattering)
typedef void (*FUNCTION_scatter_routine)(std::unique_ptr<base_particle> , const struct Functions &);


//-------------------------------------------------------------------------------------------------------
//--------------------------------------------- Logging -------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: void particle_graveyard(std::unique_ptr<base_particle> in);
typedef void (*FUNCTION_particle_graveyard)(std::unique_ptr<base_particle>);

//Used for: void logging_generic( const std::string &key, std::ostream &payload );
//typedef void (*FUNCTION_generic_logging)(const std::string &, std::ostream &);

//Used for: std::ostream & logging_generic( const std::string &key );
typedef std::ostream & (*FUNCTION_generic_logging)(const std::string &);

//-------------------------------------------------------------------------------------------------------
//---------------------------------------------- Voxels -------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Used for: void accumulate_slowdown(const double &initial_E, const vec3<double> &initial_pos,  const double &final_E, const vec3<double> &final_pos);
typedef void (*FUNCTION_accumulate_slowdown)(const double &, const vec3<double> &,  const double &, const vec3<double> &, const struct Functions &);

//Used for: void voxel_localdump(const double &E, const vec3<double> &pos);
typedef void (*FUNCTION_voxel_localdump)(const double &, const vec3<double> &, const struct Functions &);

#endif
