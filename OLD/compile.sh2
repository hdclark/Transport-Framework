#/bin/bash

COMPILER=" clang++ "
COMPILER=" g++ "
WARNINGS=" -Wall -Wunsafe-loop-optimizations " # -pedantic "
OPTIMIZATIONS=" -O3 -ffast-math -funsafe-loop-optimizations -march=native -mtune=native  -fno-signed-zeros -fno-trapping-math -fassociative-math -freciprocal-math  " # -floop-parallelize-all -floop-interchange -floop-strip-mine -floop-block "
#OPTIMIZATIONS="    "
DYNAMIC_OPTS=" -fpic -shared -Wl,-z,defs "
#EXTRAS=" -g -pg "
#EXTRAS=" -g "

# Code which needs to be linked to statically and dynamically-compiled code.
echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x -c Constants.cc -o constants.o "
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x -c Constants.cc -o constants.o 

#   (note: Misc.h doesn't need to be here because only header-defined macros are used in the dynamic libraries!)
#     ((although it wouldn't hurt anything, I think, to have it here..))

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x -c MyMath.cc -o mymath.o " 
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x -c MyMath.cc -o mymath.o  


echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x -c Structs.cc  -o structs.o "
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x -c Structs.cc  -o structs.o  


INCLUDE_EVERYWHERE=" constants.o structs.o mymath.o "
#INCLUDE_EVERYWHERE="  "


# Dynamically-loaded libraries:
echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Photons.cc ${INCLUDE_EVERYWHERE} -o lib_photons.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Photons.cc ${INCLUDE_EVERYWHERE} -o lib_photons.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Electrons.cc ${INCLUDE_EVERYWHERE} -o lib_electrons.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Electrons.cc ${INCLUDE_EVERYWHERE} -o lib_electrons.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Positrons.cc ${INCLUDE_EVERYWHERE} -o lib_positrons.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Positrons.cc ${INCLUDE_EVERYWHERE} -o lib_positrons.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Random_MT.cc ${INCLUDE_EVERYWHERE} -o lib_random_MT.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Random_MT.cc ${INCLUDE_EVERYWHERE} -o lib_random_MT.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Water_csplines.cc ${INCLUDE_EVERYWHERE} -o lib_water_csplines.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Water_csplines.cc ${INCLUDE_EVERYWHERE} -o lib_water_csplines.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Water_fitted.cc ${INCLUDE_EVERYWHERE} -o lib_water_fitted.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Water_fitted.cc ${INCLUDE_EVERYWHERE} -o lib_water_fitted.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Water_linear.cc ${INCLUDE_EVERYWHERE}  -o lib_water_linear.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Water_linear.cc ${INCLUDE_EVERYWHERE}  -o lib_water_linear.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Beam_6MV.cc ${INCLUDE_EVERYWHERE} -o lib_beam_6MV.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Beam_6MV.cc ${INCLUDE_EVERYWHERE} -o lib_beam_6MV.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Beam_6MV_Downward.cc ${INCLUDE_EVERYWHERE} -o lib_beam_6MV_downward.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Beam_6MV_Downward.cc ${INCLUDE_EVERYWHERE} -o lib_beam_6MV_downward.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Geometry.cc ${INCLUDE_EVERYWHERE} -o lib_geometry.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Geometry.cc ${INCLUDE_EVERYWHERE} -o lib_geometry.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Memory.cc ${INCLUDE_EVERYWHERE} -o lib_memory.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Memory.cc ${INCLUDE_EVERYWHERE} -o lib_memory.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Coherent.cc ${INCLUDE_EVERYWHERE} -o lib_coherent.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Coherent.cc ${INCLUDE_EVERYWHERE} -o lib_coherent.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Compton.cc ${INCLUDE_EVERYWHERE} -o lib_compton.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Compton.cc ${INCLUDE_EVERYWHERE} -o lib_compton.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Pair.cc ${INCLUDE_EVERYWHERE} -o lib_pair.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Pair.cc ${INCLUDE_EVERYWHERE} -o lib_pair.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} No_Interaction.cc ${INCLUDE_EVERYWHERE} -o lib_no_interaction.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} No_Interaction.cc ${INCLUDE_EVERYWHERE} -o lib_no_interaction.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Photoelectric.cc ${INCLUDE_EVERYWHERE} -o lib_photoelectric.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Photoelectric.cc ${INCLUDE_EVERYWHERE} -o lib_photoelectric.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Localdump.cc ${INCLUDE_EVERYWHERE} -o lib_localdump.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Localdump.cc ${INCLUDE_EVERYWHERE} -o lib_localdump.so -lm

echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Logging.cc ${INCLUDE_EVERYWHERE} -o lib_logging.so -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x ${DYNAMIC_OPTS} Logging.cc ${INCLUDE_EVERYWHERE} -o lib_logging.so -lm


# Compile-time linked code:
# Need -rdynamic so that variables in Constant.cc can be resolved through transport executable in the dynamic libraries when they are loaded!
echo "${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x Transport.cc Misc.cc Dynamic_Loading.cc ${INCLUDE_EVERYWHERE}  -o transport -ldl -lm"
      ${COMPILER}  $@ ${WARNINGS} ${EXTRAS} ${OPTIMIZATIONS} --std=c++0x Transport.cc Misc.cc Dynamic_Loading.cc ${INCLUDE_EVERYWHERE}  -o transport -ldl -lm



