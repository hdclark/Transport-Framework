//Constants.h.

#ifndef CONSTANTS_H_PROJECT_TRANSPORT
#define CONSTANTS_H_PROJECT_TRANSPORT


#ifdef __cplusplus
    extern "C" {
#endif

//----------------------------------------------------------------------------------//
//--------------------------------- Sanity Checks ----------------------------------//
//----------------------------------------------------------------------------------//
namespace LoggingQuantities {
    extern const bool MassAttenuationCoefficients;
    extern const bool MFPWaterSampling;
    extern const bool BeamSpectrumSampling;
    extern const bool CTGeometry;
    extern const bool CTDetectorGeometry;
    extern const bool CTPathMaterial;
    extern const bool PDD_6MV;
    extern const bool PDK_1MEV;
    extern const bool PDK_10MEV;
    extern const bool ElectronStoppingPos;
    extern const bool NumbOfInteractions;
    extern const bool DistanceTravelled;
    extern const bool FractionTransferredCompton;
    extern const bool PhotonAngularSampled;
    extern const bool VoxelAutoDump;
}

//----------------------------------------------------------------------------------//
//------------------------------------ Generic -------------------------------------//
//----------------------------------------------------------------------------------//

extern const unsigned int INTERACTION_COUNT_MAX_CULL;

extern const double electron_mass;
extern const double electron_charge;

extern const double positron_mass;
extern const double positron_charge;

extern const double water_mass_density;

extern const double water_binding_energy_oxygen_K;


extern const double PHOTON_SEPUKU_ENERGY_THRESHOLD; 
extern const bool PHOTON_SEPUKU_DONATE_TO_ELECTRON;
extern const bool PHOTON_SEPUKU_DISAPPEAR;
extern const bool PHOTON_SEPUKU_DISTRIBUTE;


extern const double ELECTRON_SEPUKU_ENERGY_THRESHOLD;
extern const bool ELECTRON_SEPUKU_LOCALDUMP;
//extern const bool ELECTRON_SEPUKU_DISAPPEAR;
//extern const bool ELECTRON_SEPUKU_DISTRIBUTE;


extern const double POSITRON_SEPUKU_ENERGY_THRESHOLD;
extern const bool POSITRON_SEPUKU_LOCALDUMP;      
//extern const bool POSITRON_SEPUKU_DISAPPEAR;      
//extern const bool POSITRON_SEPUKU_DISTRIBUTE;


extern const bool USE_CSDA;

extern const bool track_interactions;



//----------------------------------------------------------------------------------//
//----------------------------------- Materials ------------------------------------//
//----------------------------------------------------------------------------------//
namespace Material {
    extern const unsigned char Beam;
    extern const unsigned char Black;
    extern const unsigned char Vacuum;
    extern const unsigned char Air;
    extern const unsigned char Water;
    extern const unsigned char Detector;
    extern const unsigned char Unknown;  
}

//----------------------------------------------------------------------------------//
//--------------------------------- Particle types ---------------------------------//
//----------------------------------------------------------------------------------//
namespace Particletype {
    extern const unsigned char Photon;
    extern const unsigned char Electron;
    extern const unsigned char Positron;
}

//----------------------------------------------------------------------------------//
//------------------------------- Interaction types --------------------------------//
//----------------------------------------------------------------------------------//
namespace Interactiontype {

    extern const unsigned char Creation;

    extern const unsigned char Coherent;
    extern const unsigned char Compton;
    extern const unsigned char Photoelectric;
    extern const unsigned char Pair;

    extern const unsigned char LocalDump;     //A simple, generic interaction which dumps 
                                              // all particle energy locally and destroys
                                              // the particle.
    extern const unsigned char None;          //A 'virtual' interaction - does nothing.

    extern const unsigned char Disappear;
    extern const unsigned char Detect;

    extern const unsigned char SlowDown;

}


#ifdef __cplusplus
    }
#endif

#endif
