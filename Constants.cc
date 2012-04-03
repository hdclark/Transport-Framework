//Constants.cc - General constant global values.
//
//   **NOTE: all items here must be (truly) constant**
//
//   **NOTE: all distances are in [cm] and all energies are in [MeV].
//
//Try wrapping groups of things in a unique namespace to avoid pollution.
// Also avoid enums. They are entirely unecessary!
//

#include "./Constants.h"

//----------------------------------------------------------------------------------//
//---------------------------------- Items to log ----------------------------------//
//----------------------------------------------------------------------------------//
namespace LoggingQuantities {
    const bool MassAttenuationCoefficients = false; //Phys 539, Q1 - Directly plottable mass attenuation coefficients versus energy.
    const bool MFPWaterSampling            = false;   
    const bool BeamSpectrumSampling        = false;
    const bool CTGeometry                  = false;
    const bool CTDetectorGeometry          = false;
    const bool CTPathMaterial              = false;
    const bool PDD_6MV                     = false;  
    const bool PDK_1MEV                    = false;
    const bool PDK_10MEV                   = false;
    const bool ElectronStoppingPos         = false;
    const bool NumbOfInteractions          = false; //Phys 539, Q3 - Number of interactions a photon does before being absorbed versus initial energy.
    const bool DistanceTravelled           = false; //Distance the photon has travelled throughout the simulation. (As the crow flies!)
    const bool FractionTransferredCompton  = false; //Phys 539, Q2 - Fraction of energy transferred to recoil electrons in Compton interactions versus energy.
    const bool PhotonAngularSampled        = false; //Phys 539, Q4 - Angular sampling of the angles assigned for the recoil photon in Compton interactions.
    const bool VoxelAutoDump               =  true; //Dumps voxel data when the module is unloaded, regardless of what is contained in it.
}


//----------------------------------------------------------------------------------//
//------------------------------------ Generic -------------------------------------//
//----------------------------------------------------------------------------------//

//This controls the maximum number of interactions before culling the particles. It can
// save an awful lot of cpu time, and can be used, for example, to examine particles on 
// a central beam only. Use with care, though.
//
//NOTE: If this value is ZERO, then no culling occurs.
//NOTE: The particle creation counts as an interaction.
//NOTE: This (potentially) culls all particles, not just fresh beam particles.
//NOTE: The number of interactions req'd for a cull is > INTERACTION_COUNT_MAX_CULL.
//      In practise, this means that each culled particle will undergo this number + 1
//      interactions. IE: this is the number of interactions a particle will undergo 
//      after being created. Thus "1" will mean creation, one of {compton,pair,..}, cull.
const unsigned int INTERACTION_COUNT_MAX_CULL = 0;


const double electron_mass   = 0.510998910L;
const double electron_charge = -1.0L;

const double positron_mass   = 0.510998910L;
const double positron_charge = 1.0L;


const double water_mass_density = 1.000L; // $\rho = \frac{1\:g}{1\:cm^{3}}$

const double water_binding_energy_oxygen_K = 543.1E-6L; //Used for photoelectric effect. 543.1 eV.

//The energy level below which we begin to ignore photons. This is set by precision AND
// by data availability. Be weary of setting it too low if interpolating mass attenuation
// coefficients!
const double PHOTON_SEPUKU_ENERGY_THRESHOLD = 1E-3L;  // 1 keV.
//const double PHOTON_SEPUKU_ENERGY_THRESHOLD = 0.01;

// When a photon 'commits sepuku,' we can choose to:
// 1) have the photons 'donate' their energy to an electron, or
// 2) have the photon simply disappear, taking the remaining energy with it, or
// 3) have the photon distribute its remaining energy with some distribution throughout a region.
const bool PHOTON_SEPUKU_DONATE_TO_ELECTRON = true;
const bool PHOTON_SEPUKU_DISAPPEAR          = false;
const bool PHOTON_SEPUKU_DISTRIBUTE         = false;


//The energy level below which we begin to ignore electrons. It is closely related to 
// the equivalent number for photons, and they should roughly match lest extra 
// computation occur with no benefit (because, say, all electrons being created are
// immediately below the threshold.)
const double ELECTRON_SEPUKU_ENERGY_THRESHOLD = 0.1 + electron_mass; // 100 keV kinetic energy.
const bool ELECTRON_SEPUKU_LOCALDUMP          = true;    //Electrons will simply undergo (regular) localdump interaction.
//const bool ELECTRON_SEPUKU_DISAPPEAR          = false;
//const bool ELECTRON_SEPUKU_DISTRIBUTE         = false;

//Just follow the behaviour of the electrons.
const double POSITRON_SEPUKU_ENERGY_THRESHOLD = ELECTRON_SEPUKU_ENERGY_THRESHOLD;
const bool POSITRON_SEPUKU_LOCALDUMP          = ELECTRON_SEPUKU_LOCALDUMP;
//const bool POSITRON_SEPUKU_DISAPPEAR          = ELECTRON_SEPUKU_DISAPPEAR;
//const bool POSITRON_SEPUKU_DISTRIBUTE         = ELECTRON_SEPUKU_DISTRIBUTE;


//We can choose to either have electrons dump their energies on-spot ("LocalDump")
// or continue along their trajectory while emitting energy continuously, until they
// 'evaporate'. This is a soft-collision approximation.
//
//It is probably more accurate to turn this on (but how much more?)
const bool USE_CSDA = true; 

//We can choose to have each particle track its own interaction history. This means that
// each particle carries around an expandable history - bad news for large numbers of 
// particles. Turn this off if memory climbs and exhausts for large particle counts.
const bool track_interactions = true;

//----------------------------------------------------------------------------------//
//----------------------------------- Materials ------------------------------------//
//----------------------------------------------------------------------------------//
namespace Material {

    const unsigned char Beam     = 1;   //Particles created in a beam start in this medium.
    const unsigned char Black    = 2;   //Black-hole - it outright absorbs all particles.
    const unsigned char Vacuum   = 3;   
    const unsigned char Air      = 4;
    const unsigned char Water    = 5;
    const unsigned char Detector = 6;
    const unsigned char Unknown  = 7; //Used for creating particles in a medium when I don't care about the material it is in. I will fix this when I need it/get a chance.
}

//----------------------------------------------------------------------------------//
//--------------------------------- Particle types ---------------------------------//
//----------------------------------------------------------------------------------//
namespace Particletype {
 
    const unsigned char Photon   = 1;
    const unsigned char Electron = 2;
    const unsigned char Positron = 3;

}  

//----------------------------------------------------------------------------------//
//------------------------------- Interaction types --------------------------------//
//----------------------------------------------------------------------------------//
namespace Interactiontype {

    const unsigned char Creation         = 1;

    const unsigned char Coherent         = 2;
    const unsigned char Compton          = 3;
    const unsigned char Photoelectric    = 4;
    const unsigned char Pair             = 5;

    const unsigned char LocalDump        = 6;
    const unsigned char None             = 7;
    const unsigned char Disappear        = 8; //Particle is completely removed. Breaks all conservations!

    const unsigned char Detect           = 9;

    const unsigned char SlowDown         = 10;
}
