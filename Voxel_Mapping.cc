//Voxel_Mapping.cc - Provides functions for scoring the geometry into voxels. This is a simplistic version which does *not* speak to the Geometry file.
//                   This file is equipped to score LocalDump and SlowDown scatter routines.
//
//Programming notes:
//  -Do not make items here "const", because they will not show up when loading.
//  -Avoid using macro variables here because they will be obliterated during loading.
//  -Wrap dynamically-loaded code with extern "C", otherwise C++ compilation will mangle function names, etc.
//
// From man page for dlsym/dlopen:  For running some 'initialization' code prior to finishing loading:
// "Instead,  libraries  should  export  routines using the __attribute__((constructor)) and __attribute__((destructor)) function attributes.  See the gcc info pages for
//       information on these.  Constructor routines are executed before dlopen() returns, and destructor routines are executed before dlclose() returns."
//   ---for instance, we can use this to seed a random number generator with a random seed. However, in order to pass in a specific seed (and pass that seed to the library)
//      we need to define an explicitly callable initialization function. In general, these libraries should have both so that we can quickly adjust behaviour if desired.
//  

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <memory>
#include <cmath>

#include "./Misc.h"
#include "./MyMath.h"

#include "./Constants.h"
#include "./Structs.h"

#ifdef __cplusplus
    extern "C" {
#endif

std::string MODULE_NAME(__FILE__);
std::string FILE_TYPE("VOXEL");

bool VERBOSE = false;



struct voxel {
    long int photon_primary_interactions;
    double accumulated_dose;
    double accumulated_kerma;
    double Etransferred;
    voxel():photon_primary_interactions(0),accumulated_dose(0.0),accumulated_kerma(0.0),Etransferred(0.0) { }
};



vec3<long int> voxel_coords;
voxel data[60][60][100]; // <---- x,y,z
bool  mask[60][60][100];
double   max_dose, max_kerma;   //Used for normalization - dose or kerma.
long int max_count;   //Used for normalization - number of primary events.

#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        for(unsigned long i = 0; i < 60; ++i) for(unsigned long j = 0; j < 60; ++j) for(unsigned long k = 0; k < 100; ++k) mask[i][j][k] = false;

        max_dose  = 0.0;
        max_kerma = 0.0;
        max_count = 0;

        if(VERBOSE) FUNCINFO("Loaded lib_voxel_mapping.so");
        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.

        if(LoggingQuantities::VoxelAutoDump){

            //Primary Events.
            for(long int k=0; k<100; ++k){
                //Export the data to file. Use the P3 PPM file format (http://en.wikipedia.org/wiki/Netpbm_format) because it is so easy to use.
                std::fstream FO;
                std::string filename = "/tmp/Transport_primary_events_";
                if(k<10) filename += "0";
                if(k<100) filename += "0";
                filename += Xtostring<long int>(k) + ".ppm";
                
                FO.open(filename.c_str(), std::fstream::out);
        
                //For primary events.
                FO << "P3" << std::endl;  //This is required for the file to be recognized as a PPM file.
                FO << "# This is a dose file slice at depth " << k << " of 100" << std::endl;
                FO << "60" << " " << "60" << std::endl; //Columns and rows.
                FO << "# now we give the maximum value of the RGB coordinates." << std::endl;
                FO << max_count << std::endl;
                
                for(size_t i=0; i<60; ++i){
                    for(size_t j=0; j<60; ++j){
                        FO << (data[i][j][k]).photon_primary_interactions << " " << "0" << " " << "0" << " ";
                    }
                    FO << std::endl;
                } 
                FO.close();
            }

            //Dose.
            for(long int k=0; k<100; ++k){
                std::fstream FO;
                std::string filename = "/tmp/Transport_dose_";
                if(k<10) filename += "0";
                if(k<100) filename += "0";
                filename += Xtostring<long int>(k) + ".ppm";
 
                FO.open(filename.c_str(), std::fstream::out);
 
                //For primary events.
                FO << "P3" << std::endl;  //This is required for the file to be recognized as a PPM file.
                FO << "# This is a dose file slice at depth " << k << " of 100" << std::endl;
                FO << "60" << " " << "60" << std::endl; //Columns and rows.
                FO << "# now we give the maximum value of the RGB coordinates." << std::endl;
                FO << static_cast<long int>(max_dose) << std::endl;

                for(size_t i=0; i<60; ++i){
                    for(size_t j=0; j<60; ++j){
                        FO << static_cast<long int>((data[i][j][k]).accumulated_dose) << " " << "0" << " " << "0" << " ";
                    }
                    FO << std::endl;
                }
                FO.close();
            }

            //Kerma.
            for(long int k=0; k<100; ++k){
                std::fstream FO;
                std::string filename = "/tmp/Transport_kerma_";
                if(k<10) filename += "0";
                if(k<100) filename += "0";
                filename += Xtostring<long int>(k) + ".ppm";

                FO.open(filename.c_str(), std::fstream::out);

                //For primary events.
                FO << "P3" << std::endl;  //This is required for the file to be recognized as a PPM file.
                FO << "# This is a kerma file slice at depth " << k << " of 100" << std::endl;
                FO << "60" << " " << "60" << std::endl; //Columns and rows.
                FO << "# now we give the maximum value of the RGB coordinates." << std::endl;
                FO << static_cast<long int>(max_kerma) << std::endl;

                for(size_t i=0; i<60; ++i){
                    for(size_t j=0; j<60; ++j){
                        FO << static_cast<long int>((data[i][j][k]).accumulated_kerma) << " " << "0" << " " << "0" << " ";
                    }
                    FO << std::endl;
                }
                FO.close();
            }

            //Etransfer.
            for(long int k=0; k<100; ++k){
                std::fstream FO;
                std::string filename = "/tmp/Transport_Etransferred_";
                if(k<10) filename += "0";
                if(k<100) filename += "0";
                filename += Xtostring<long int>(k) + ".ppm";

                FO.open(filename.c_str(), std::fstream::out);

                //For primary events.
                FO << "P3" << std::endl;  //This is required for the file to be recognized as a PPM file.
                FO << "# This is a Etransferred file slice at depth " << k << " of 100" << std::endl;
                FO << "60" << " " << "60" << std::endl; //Columns and rows.
                FO << "# now we give the maximum value of the RGB coordinates." << std::endl;
                FO << "123" << std::endl;

                for(size_t i=0; i<60; ++i){
                    for(size_t j=0; j<60; ++j){
                        FO << static_cast<long int>((data[i][j][k]).Etransferred) << " " << "0" << " " << "0" << " ";
                    }
                    FO << std::endl;
                }
                FO.close();
            }


        } 

        if(VERBOSE) FUNCINFO("Closed lib_voxel_mapping.so");
        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}

bool to_voxel_coords(const vec3<double> &in){  //This is a STATEFUL function. It updates the global pixel coordinate vector to reduce contructor overhead.
                                               // Returns true only if the conversion to voxel coordinates fails.
    //Check the bounding box.
    if((fabs(in.x) > 15.0) || (fabs(in.y) > 15.0) || (in.z > 0.0) || (in.z < -50.0)) return false;

    //Perform the transformation.
    const long int x = (long int)rint((in.x + 15.0)/0.5);
    const long int y = (long int)rint((in.y + 15.0)/0.5);
    const long int z = (long int)rint((-in.z)/0.5);

    //Final check.
    if((x < 0) || (x >= 60) || (y < 0) || (y >= 60) || (z < 0) || (z >= 100)) return false;

    //Commit the coordinates and return.
    voxel_coords = vec3<long int>( x, y, z );
   
    return true;
}



void accumulate_slowdown(const double &initial_E, const vec3<double> &initial_pos,  const double &final_E, const vec3<double> &final_pos, const struct Functions &Loaded_Funcs){

/*
    //Troubleshooting.
    std::cout << "Performed a slowdown accumulation with E,R = " << initial_E << ", " << initial_pos << " --> " << final_E << ", " << final_pos << ". ";
    if( to_voxel_coords( initial_pos ) ){
        std::cout << " Within voxel geom. Coords are: " << voxel_coords << std::endl;
        return;
    }
    std::cout << " Outside voxel geom. " << std::endl;
*/


    //Get a unit vector in the direction of travel and the distance to travel.
    vec3<double> path = final_pos;
    path -= initial_pos;

    const double distance  = path.length();
    vec3<double> dir       = path.unit();
    const double Elost     = initial_E - final_E;

    //Register the primary event, if it occurs inside the voxel geometry.
    if( to_voxel_coords( initial_pos ) ){
        ++(data[voxel_coords.x][voxel_coords.y][voxel_coords.z].photon_primary_interactions);

        data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_kerma += Elost;

                 double probable_photon_E = 6.0*(initial_E - electron_mass);
                 if( probable_photon_E > 50.0) probable_photon_E = 49.9;

        data[voxel_coords.x][voxel_coords.y][voxel_coords.z].Etransferred += probable_photon_E * 
                 (Loaded_Funcs.photon_mass_coefficient_transfer(probable_photon_E) / Loaded_Funcs.photon_mass_coefficient_total(probable_photon_E) );
 
        //Used for normalization afterward.
        if(data[voxel_coords.x][voxel_coords.y][voxel_coords.z].photon_primary_interactions > max_count){
            max_count = data[voxel_coords.x][voxel_coords.y][voxel_coords.z].photon_primary_interactions;
        }

        if(max_kerma < data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_kerma){
            max_kerma = data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_kerma;
        }


    }


    //Crawl along the path and deposit energy.
    vec3<double> pos = initial_pos;
    double dx = 0.24;  //Should be smaller than the linear dimensions of the voxel.
    if(2.0*dx > distance) dx = 0.34*distance;
    for(double x = 0.0; x <= distance; x += dx){
        pos += dir * dx;
        if( to_voxel_coords( pos ) ){

            //Accumulate the quantities required.
            data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_dose  += (dx/distance)*Elost;

            //Running maximum value to save us having to determine it later.
            if(max_dose < data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_dose){
                max_dose = data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_dose;
            }

        }
    }


    return;
}


void voxel_localdump(const double &T, const vec3<double> &pos, const struct Functions &Loaded_Funcs){ //Requires kinetic energy because it cannot tell which particle is being dumped!
    //This function takes a localdump event and registers it in a single voxel.
    if( to_voxel_coords( pos ) ){

           //Accumulate the quantities required.
            data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_dose  += T;

            data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_kerma += T;

                     double probable_photon_E = 6.0*T ;
                     if(probable_photon_E > 50.0) probable_photon_E = 49.9;

            data[voxel_coords.x][voxel_coords.y][voxel_coords.z].Etransferred += probable_photon_E * 
                 (Loaded_Funcs.photon_mass_coefficient_transfer(probable_photon_E) / Loaded_Funcs.photon_mass_coefficient_total(probable_photon_E) );


            //Running maximum value to save us having to determine it later.
            if(max_dose < data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_dose){
                max_dose = data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_dose;
            }
            if(max_kerma < data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_kerma){
                max_kerma = data[voxel_coords.x][voxel_coords.y][voxel_coords.z].accumulated_kerma;
            }
    }

    return;
}



#ifdef __cplusplus
    }
#endif

