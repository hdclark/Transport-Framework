//Logging.cc - Routines used for simple tracking of general information. These are not software logs - they are physics logs!
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
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <utility>  //std::pair
#include <memory>   //std::unique_ptr
#include <cmath>

#include "./Misc.h"

#include "./MyMath.h"
#include "./Constants.h"
#include "./Structs.h"

#ifdef __cplusplus
    extern "C" {
#endif

std::string MODULE_NAME(__FILE__);
std::string FILE_TYPE("LOGGING");

bool VERBOSE = false;
bool DO_NOT_CLOBBER = false;

std::map<std::string, std::pair<std::string, std::unique_ptr<std::fstream> > >   Log_File;  //Log file (1) key values, (2) filenames, (3) file descriptors.
std::map<std::string, std::pair<std::string, std::unique_ptr<std::fstream> > >::iterator Log_File_Iter; //A convenience iterator.

/*
                    FO.open(FilenameOut.c_str(), std::ifstream::out);
                    if( FO.fail() ){
                        FUNCWARN("Unable to open output file \"" << FilenameOut << "\". Aborting output attempt. ");
                        return;
                    }

                    FO << "# Dose profile: " << std::endl;
*/


#ifdef __GNUG__
    __attribute__((constructor)) static void init_on_dynamic_load(void){
        //Do something automatic here.
        if(VERBOSE) FUNCINFO("Loaded lib_logging.so");
/*
//-----------------
//Phys 539 geometry
        //Push back all the logging file filenames.
        Log_File["Other"] = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Other.process",  std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["Q1"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Ques_1.process", std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["Q2"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Ques_2.process", std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["Q3"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Ques_3.process", std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["Q4"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Ques_4.process", std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["Q5"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Ques_5.process", std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["Q6"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Ques_6.process", std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["Q7"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Ques_7.process", std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["Q8"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Ques_8.process", std::unique_ptr<std::fstream>( new std::fstream ) );

        Log_File["elec_stop"] = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_electron_stop.process", std::unique_ptr<std::fstream>( new std::fstream ) );

        Log_File["PD_Kerma_1MeV"] = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_PD_Kerma_1MeV.process", std::unique_ptr<std::fstream>( new std::fstream ) );
        Log_File["PD_Kerma_10MeV"] = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_PD_Kerma_10MeV.process", std::unique_ptr<std::fstream>( new std::fstream ) );

        Log_File["PD_Dose_6MV"] = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_PD_Dose_6MV.process", std::unique_ptr<std::fstream>( new std::fstream ) );


//Detector geometry
        Log_File["Detector"]    = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_Detector.process", std::unique_ptr<std::fstream>( new std::fstream ) );
//-----------------


        //Open the files for logging. Warn if they already exist.
        for( Log_File_Iter = Log_File.begin(); Log_File_Iter != Log_File.end(); Log_File_Iter++){
            //std::string the_key = Log_File_Iter->first;
            //std::pair<std::string, std::fstream> the_pair = Log_File_Iter->second;

            //If DO_NOT_CLOBBER is set, we bail out if any of the output files exists. This is fairly cumbersome to use in practice, unless launching with a script.
            if(DO_NOT_CLOBBER){
                //FO.open(FilenameOut.c_str(), std::ifstream::in);
                (  (Log_File_Iter->second).second  )->open(   (Log_File_Iter->second).first  , std::ifstream::in);
                //if( FO.fail() ){
                if( !((Log_File_Iter->second).second)->fail() ){
                    ((Log_File_Iter->second).second)->close();
                    FUNCERR("Logging file \"" << (Log_File_Iter->second).first << "\" exists. Unable to proceed.");
                }
                
            }

            //Open for writing.
            (  (Log_File_Iter->second).second  )->open(   (Log_File_Iter->second).first  , std::ifstream::out);
            if( ((Log_File_Iter->second).second)->fail() ){
                FUNCERR("Logging file \"" << (Log_File_Iter->second).first << "\" cannot be open for writing. Unable to proceed.");
            }

            //The file descriptor should stay open for the remainder of the program. It will be closed when we unload the library.

        }      
*/


/*

//-----------------
//Phys 539 geometry

        //Prepare headers for the files, if necessary.
        *(Log_File["Q1"].second) << "# Mass attenuation coefficients. " << std::endl;
        *(Log_File["Q1"].second) << "#   Energy   Coherent   Compton   Photoelectric   Pair   Total " << std::endl;

        *(Log_File["Q2"].second) << "# Fraction of energy transferred to recoil electrons (in Compton interactions) as a function of the incident photon energy. " << std::endl;
        *(Log_File["Q2"].second) << "#   Incident_Energy   Fraction:(Electron_energy/Incident_Energy)" << std::endl;

        *(Log_File["Q3"].second) << "# Number of interactions a photon underwent until it was fully absorbed versus initial (beam) energy." << std::endl;
        *(Log_File["Q3"].second) << "#   Energy   Total " << std::endl;

        *(Log_File["Q4"].second) << "# Angular distribution of the Kelin-Nishina distribution as a function of the incident photon energy." << std::endl;
        *(Log_File["Q4"].second) << "#   Energy   theta (photon scatering angle)  phi (angle about the line of the incoming photon) " << std::endl;

        *(Log_File["elec_stop"].second) << "# The position (in real-space) where electrons are fully stopped (as far as the program is concerned) in a medium." << std::endl;
        *(Log_File["elec_stop"].second) << "# This data is used for debugging. In more realistic situations, one would be more interested in dose delivered to a medium as it travelled." << std::endl;
        *(Log_File["elec_stop"].second) << "#    x     y     z     r (from 0,0,0)     E (at time of absorption - may not be accurate if electron-particle interactions are turned on..) " << std::endl;

        *(Log_File["PD_Kerma_1MeV"].second) << "# This is a measure of the distance which a photon of 1 MeV energy (at time of creation) has travelled into a medium until an interaction occurs." << std::endl;
        *(Log_File["PD_Kerma_1MeV"].second) << "# Since we specifically examine a monoenergetic portion of the spectrum, we can compute the Percent-Depth Kerma simply by binning and normalizing these distances." << std::endl;
        *(Log_File["PD_Kerma_1MeV"].second) << "#   distance from point of creation (cm) " << std::endl;

        *(Log_File["PD_Kerma_10MeV"].second) << "# This is a measure of the distance which a photon of 10 MeV energy (at time of creation) has travelled into a medium until an interaction occurs." << std::endl;
        *(Log_File["PD_Kerma_10MeV"].second) << "# Since we specifically examine a monoenergetic portion of the spectrum, we can compute the Percent-Depth Kerma simply by binning and normalizing these distances." << std::endl;
        *(Log_File["PD_Kerma_10MeV"].second) << "#   distance from point of creation (cm) " << std::endl;

        *(Log_File["PD_Dose_6MV"].second) << "# This file can be parsed to give the (arbitrarily normalized) photon fluence at depth. Integrated properly, it will give the (one-dimensional) depth-dose profile" << std::endl;
        *(Log_File["PD_Dose_6MV"].second) << "# for a 6MV spectrum. " << std::endl;
        *(Log_File["PD_Dose_6MV"].second) << "#   distance from point of creation (cm)   photon energy     (total_mass_attenuation_coefficient*average_energy_absorbed)(photon energy)" << std::endl;

//Detector geometry
        *(Log_File["Detector"].second) << "# energy  x  y  z  #_of_interactions " << std::endl;
//-----------------
*/

        return;
    }

    __attribute__((destructor)) static void cleanup_on_dynamic_unload(void){
        //Cleanup memory (if needed) automatically here.
        if(VERBOSE) FUNCINFO("Closed lib_logging.so");

        //Close all file descriptors. None should be closed already (nor will it matter if they are at this point...)
        for( Log_File_Iter = Log_File.begin(); Log_File_Iter != Log_File.end(); Log_File_Iter++){
            //std::string the_key = Log_File_Iter->first;
            //std::pair<std::string, std::fstream> the_pair = Log_File_Iter->second;

            //FO.flush();
            ((Log_File_Iter->second).second)->flush();

            //FO.close();
            ((Log_File_Iter->second).second)->close();
        }      

        return;
    }
#else
    #warning Being compiled with non-gcc compiler. Unable to use gcc-specific function declarations like 'attribute.' Proceed at your own risk!
#endif 

void toggle_verbosity(bool in){
    VERBOSE = in;
    return;
}


std::ostream & logging_generic( const std::string &key ){

    //Check if the key corresponds to a valid file descriptor.
    if( Log_File.find( key ) == Log_File.end() ){
        //We have several options for handling failure:
        //   1. Write it to stdout and issue a warning (maybe)
        //   2. Store it until the end in a list which corresponds to the key used. Ask the human what to do with it.
        //   3. Use the key as a filename (with a common prefix for the output) and open the file when it is first requested.
        //   4. Ignore it. Possibly complain that it was ignored.
       
    /* 
        //Option 1.
        //
        //Complain. Maybe we should write the message to a generic log as well?
        FUNCWARN("Unable to find log file corresponding to key \"" << key << "\". Passing std::cout in lieu!");
        return std::cout; 
    */

        //Option 2.
        // 
        //Not implemented. Maybe when some level of interactivity is desired (not at the moment...)

    
        //Option 3.
        //
        //We will treat the key as a filename suffix. Performance hit will probably not matter too much. 
        // We will close these when the module is unloaded.
        Log_File[key] = std::pair<std::string, std::unique_ptr<std::fstream> >( "/tmp/Transport_" + key + ".process", std::unique_ptr<std::fstream>( new std::fstream ) );
        
        //If DO_NOT_CLOBBER is set, we bail out if any of the output files exists. This is fairly cumbersome to use in practice, unless launching with a script.
        if(DO_NOT_CLOBBER){
            //FO.open(FilenameOut.c_str(), std::ifstream::in);
            ((Log_File[key]).second)->open(  (Log_File[key]).first , std::ifstream::in);
            //if( FO.fail() ){
            if( !((Log_File[key]).second)->fail() ){
                ((Log_File[key]).second)->close();
                FUNCERR("Logging file \"" << (Log_File[key]).first << "\" exists. Unable to open file. Unwilling to direct data to stdout. Exiting");
                //Much better : simply make the stream redirect to stdout. I have not yet figured out how to do this (failsafe-ly)...
            }
        }

        //Open for writing.
        ((Log_File[key]).second)->open(  (Log_File[key]).first , std::ifstream::out);
        if( ((Log_File[key]).second)->fail() ){
            FUNCERR("Logging file \"" << (Log_File[key]).first << "\" cannot be open for writing. Unwilling to direct data to stdout. Exiting");
            //Much better : simply make the stream redirect to stdout. I have not yet figured out how to do this (failsafe-ly)...
        }
    

        //Option 4.
        //
        //This is not possible (that I know of) since this function is *required* to return a valid std::ostream. Use another option!



        //--------------------------------------------------
        //We now fill some files with headers. This is not necessary, but avoids awkwardly writing headers to files which never are used otherwise.
        if(key == "Mass_Attenuation_Coefficients"){
            logging_generic("Mass_Attenuation_Coefficients") << "# Mass attenuation coefficients. " << std::endl;
            logging_generic("Mass_Attenuation_Coefficients") << "#   Energy   Coherent   Compton   Photoelectric   Pair   Total " << std::endl;

        }else if(key == "Fraction_Transferred_Compton"){
            logging_generic("Fraction_Transferred_Compton") << "# Fraction of energy transferred to recoil electrons (in Compton interactions) as a function of the incident photon energy. " << std::endl;
            logging_generic("Fraction_Transferred_Compton") << "#   Incident_Energy   Fraction:(Electron_energy/Incident_Energy)" << std::endl;

        }else if(key == "Interaction_Count"){
            logging_generic("Interaction_Count") << "# Number of interactions a photon underwent until it was fully absorbed versus initial (beam) energy." << std::endl;
            logging_generic("Interaction_Count") << "#   Energy   Total " << std::endl;

        }else if(key == "Photon_Angular_Distribution"){
            logging_generic("Photon_Angular_Distribution") << "# Angular distribution of the Kelin-Nishina distribution as a function of the incident photon energy." << std::endl;
            logging_generic("Photon_Angular_Distribution") << "#   Energy   theta (photon scatering angle)  phi (angle about the line of the incoming photon) " << std::endl;

        }else if(key == "Electron_Stopped"){
            logging_generic("Electron_Stopped") << "# The position (in real-space) where electrons are fully stopped (as far as the program is concerned) in a medium." << std::endl;
            logging_generic("Electron_Stopped") << "# This data is used for debugging. In more realistic situations, one would be more interested in dose delivered to a medium as it travelled." << std::endl;
            logging_generic("Electron_Stopped") << "#    x     y     z     r (from 0,0,0)     E (at time of absorption - may not be accurate if electron-particle interactions are turned on..) " << std::endl;

        }else if(key == "PD_Kerma_1MeV"){
            logging_generic("PD_Kerma_1MeV") << "# This is a measure of the distance which a photon of 1 MeV energy (at time of creation) has travelled into a medium until an interaction occurs." << std::endl;
            logging_generic("PD_Kerma_1MeV") << "# Since we specifically examine a monoenergetic portion of the spectrum, we can compute the Percent-Depth Kerma simply by binning and normalizing these distances." << std::endl;
            logging_generic("PD_Kerma_1MeV") << "#   distance from point of creation (cm) " << std::endl;
    
        }else if(key == "PD_Kerma_10MeV"){
            logging_generic("PD_Kerma_10MeV") << "# This is a measure of the distance which a photon of 10 MeV energy (at time of creation) has travelled into a medium until an interaction occurs." << std::endl;
            logging_generic("PD_Kerma_10MeV") << "# Since we specifically examine a monoenergetic portion of the spectrum, we can compute the Percent-Depth Kerma simply by binning and normalizing these distances." << std::endl;
            logging_generic("PD_Kerma_10MeV") << "#   distance from point of creation (cm) " << std::endl;
    
        }else if(key == "PD_Dose_6MV"){
            logging_generic("PD_Dose_6MV") << "# This file can be parsed to give the (arbitrarily normalized) photon fluence at depth. Integrated properly, it will give the (one-dimensional) depth-dose profile" << std::endl;
            logging_generic("PD_Dose_6MV") << "# for a 6MV spectrum. " << std::endl;
            logging_generic("PD_Dose_6MV") << "#   distance from point of creation (cm)   photon energy     (total_mass_attenuation_coefficient*average_energy_absorbed)(photon energy)" << std::endl;
    
        }else if(key == "Detector"){
            logging_generic("Detector") << "# energy  x  y  z  #_of_interactions " << std::endl;
        }    

    }

    return *((Log_File[key]).second);
} 




//Instead of "expiring" references to inactive particles, they should be sent here so they can be recorded.
//
//This function assumes that whatever kinetic energy the particle has is delivered where the particle is
// currently located.
void particle_graveyard(std::unique_ptr<base_particle> in){

//    std::cout << "LOG: Received a particle of type: " << (int)(in->get_type()) << " at position " << in->get_position3() << " with energy " << in->get_energy() << std::endl;


    if((in->get_type() == Particletype::Photon) && (in->Interactions.size() != 0)){

        //Log question 3 - data about number of interactions a photon does before being absorbed versus initial energy.
        if(LoggingQuantities::NumbOfInteractions){
            logging_generic("Interaction_Count") << in->Interactions[0].energy << " " << (in->Interactions.size() - 1) << std::endl;  //The photon creation is of no interest to us.
        }

        //Distance, as the crow flies, from point of creation to current position.
        if(LoggingQuantities::DistanceTravelled){
            vec3<double> dist = in->Interactions[0].position;
            dist -= in->Interactions[ (in->Interactions.size() - 1) ].position;
            logging_generic("Other") << in->Interactions[0].energy << " " << sqrt(dist.x*dist.x + dist.y*dist.y + dist.z*dist.z ) << std::endl;
        }

    }else if( (in->get_type() == Particletype::Electron) || (in->get_type() == Particletype::Positron) ){

/*   moved to Localdump, where it should be.
        vec3<double> pos = in->get_position3(); 
        *(Log_File["elec_stop"].second)  << pos.x << " " << pos.y << " " << pos.z << " " << sqrt(pos.x*pos.x+pos.y*pos.y+pos.z*pos.z) << " " << in->get_energy() << std::endl;
*/

    }


    //The particle will now evaporate!
    return;
}








#ifdef __cplusplus
    }
#endif

