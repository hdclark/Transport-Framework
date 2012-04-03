# Makefile for Project - Transport, written in 2012 by haley clark.
#
# See copyright file for information about copyright and licenses.

SHELL := /bin/bash
CC    = clang++ # ccache clang++
CC    = g++ # ccache g++
CC    = $( command -v ccache >/dev/null 2>&1 && { echo "ccache" >&2; exit 0; } || command -v clang++ >/dev/null 2>&1 ||{ echo "g++" >&2; exit 1; } )
CC    =  ccache g++
###############################################################################

DEFINITIONS   = #-D

CPPFLAGS      = -std=c++0x 

OPTIMIZATIONS = -O3 -ffast-math -funsafe-loop-optimizations -march=native -mtune=native \
                -fno-signed-zeros -fno-trapping-math -fassociative-math -freciprocal-math \
                # -floop-parallelize-all -floop-interchange -floop-strip-mine -floop-block 

WARNINGS      =  -Wall -Wextra -Wunsafe-loop-optimizations \
                 # -ggdb# -Wmissing-include-dirs -Wsuggest-attribute=pure \
                 -Wsuggest-attribute=const -Wsuggest-attribute=noreturn \
                 -Wunsafe-loop-optimizations -Wconversion -Wdisabled-optimization #-Wdouble-promotion

COMPILRINFO   = #-p -pg -ftime-report -fmem-report -Q

LIB_PATHS     = -I/usr/include/ -I/usr/local/include

COMMON        = ${DEFINITIONS} ${CPPFLAGS} ${OPTIMZTNS} ${LIB_PATHS} ${WARNINGS} ${COMPILRINFO}

###############################################################################

DYNAMIC_OPTS  = -fpic -shared -Wl,-z,defs

###############################################################################

COMMON_LIBS   = -lm

ALL_LIBS      = ${COMMON_LIBS} 

###############################################################################

COMMON_SOURCES_O = constants.o mymath.o structs.o 
COMMON_SOURCES_H = Constants.h MyMath.h Structs.h

SHARED_OBJECTS = lib_photons.so lib_electrons.so lib_positrons.so lib_random_MT.so lib_water_csplines.so \
                 lib_water_fitted.so lib_water_linear.so lib_beam_6MV.so        \
                 lib_beam_xray_N7599.so lib_beam_1MeV_photons.so lib_beam_10MeV_photons.so \
                 lib_geometry_inf_water.so lib_geometry_water_slab.so  lib_geometry_water_tank.so \
                 lib_geometry_CT_imager.so lib_detect.so lib_slowdown.so \
                 lib_memory.so lib_coherent.so lib_compton.so lib_pair.so      \
                 lib_no_interaction.so lib_photoelectric.so lib_localdump.so lib_logging.so \
                 lib_voxel_mapping.so

.PHONY: all

all: transport ${SHARED_OBJECTS}

run: all
	time ./transport


# Executables.
transport: ${COMMON_SOURCES_O} Dynamic_Loading.h Typedefs.h Transport.cc Misc.cc
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} Transport.cc Misc.cc ${COMMON_SOURCES_O}  Dynamic_Loading.cc -o transport -ldl ${ALL_LIBS} 

 
# Common sources.
constants.o: Constants.cc Constants.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} -c Constants.cc -o constants.o ${ALL_LIBS}

mymath.o: MyMath.cc MyMath.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} -c MyMath.cc -o mymath.o ${ALL_LIBS}

structs.o: Structs.cc Structs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} -c Structs.cc -o structs.o ${ALL_LIBS}


# Shared libraries.
lib_photons.so: Photons.cc  ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} 
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Photons.cc ${COMMON_SOURCES_O} -o lib_photons.so ${ALL_LIBS}

lib_electrons.so: Electrons.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Electrons.cc ${COMMON_SOURCES_O} -o lib_electrons.so ${ALL_LIBS}

lib_positrons.so: Positrons.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Positrons.cc ${COMMON_SOURCES_O} -o lib_positrons.so ${ALL_LIBS}

lib_random_MT.so: Random_MT.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Random_MT.cc ${COMMON_SOURCES_O} -o lib_random_MT.so ${ALL_LIBS}

lib_water_csplines.so: Water_csplines.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Water_csplines.cc ${COMMON_SOURCES_O} -o lib_water_csplines.so ${ALL_LIBS}

lib_water_fitted.so: Water_fitted.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Water_fitted.cc ${COMMON_SOURCES_O} -o lib_water_fitted.so ${ALL_LIBS}

lib_water_linear.so: Water_linear.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Water_linear.cc ${COMMON_SOURCES_O} -o lib_water_linear.so ${ALL_LIBS}

lib_beam_1MeV_photons.so: Beam_1MeV_Photons.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Beam_1MeV_Photons.cc ${COMMON_SOURCES_O} -o lib_beam_1MeV_photons.so ${ALL_LIBS}

lib_beam_10MeV_photons.so: Beam_10MeV_Photons.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Beam_10MeV_Photons.cc ${COMMON_SOURCES_O} -o lib_beam_10MeV_photons.so ${ALL_LIBS}

lib_beam_6MV.so: Beam_6MV.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Beam_6MV.cc ${COMMON_SOURCES_O} -o lib_beam_6MV.so ${ALL_LIBS}

lib_beam_xray_N7599.so: Beam_Xray_N7599.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Beam_Xray_N7599.cc ${COMMON_SOURCES_O} -o lib_beam_xray_N7599.so ${ALL_LIBS}

lib_geometry_inf_water.so: Geometry_Inf_Water.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Geometry_Inf_Water.cc ${COMMON_SOURCES_O} -o lib_geometry_inf_water.so ${ALL_LIBS}

lib_geometry_water_slab.so: Geometry_Water_Slab.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Geometry_Water_Slab.cc ${COMMON_SOURCES_O} -o lib_geometry_water_slab.so ${ALL_LIBS}

lib_geometry_CT_imager.so: Geometry_CT_Imager.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Geometry_CT_Imager.cc ${COMMON_SOURCES_O} -o lib_geometry_CT_imager.so ${ALL_LIBS}

lib_geometry_water_tank.so: Geometry_Water_Tank.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Geometry_Water_Tank.cc ${COMMON_SOURCES_O} -o lib_geometry_water_tank.so ${ALL_LIBS}

lib_memory.so: Memory.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Memory.cc ${COMMON_SOURCES_O} -o lib_memory.so ${ALL_LIBS}

lib_coherent.so: Coherent.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} Typedefs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Coherent.cc ${COMMON_SOURCES_O} -o lib_coherent.so ${ALL_LIBS}

lib_compton.so: Compton.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} Typedefs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Compton.cc ${COMMON_SOURCES_O} -o lib_compton.so ${ALL_LIBS}

lib_pair.so: Pair.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} Typedefs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Pair.cc ${COMMON_SOURCES_O} -o lib_pair.so ${ALL_LIBS}

lib_no_interaction.so: No_Interaction.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} Typedefs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} No_Interaction.cc ${COMMON_SOURCES_O} -o lib_no_interaction.so ${ALL_LIBS}

lib_photoelectric.so: Photoelectric.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} Typedefs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Photoelectric.cc ${COMMON_SOURCES_O} -o lib_photoelectric.so ${ALL_LIBS}

lib_localdump.so: Localdump.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} Typedefs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Localdump.cc ${COMMON_SOURCES_O} -o lib_localdump.so ${ALL_LIBS}

lib_slowdown.so: SlowDown.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} Typedefs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} SlowDown.cc ${COMMON_SOURCES_O} -o lib_slowdown.so ${ALL_LIBS}

lib_detect.so: Detect.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H} Typedefs.h
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Detect.cc ${COMMON_SOURCES_O} -o lib_detect.so ${ALL_LIBS}

lib_logging.so: Logging.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Logging.cc ${COMMON_SOURCES_O}  -o lib_logging.so ${ALL_LIBS}

lib_voxel_mapping.so: Voxel_Mapping.cc ${COMMON_SOURCES_O} ${COMMON_SOURCES_H}
	${CC} ${COMMON} ${WARNINGS} ${OPTIMIZATIONS} ${DYNAMIC_OPTS} Voxel_Mapping.cc Misc.cc ${COMMON_SOURCES_O}  -o lib_voxel_mapping.so ${ALL_LIBS}




clean: 
	for i in ${COMMON_SOURCES_O} $(wildcard lib_*so) transport ; do \
            test -f "$${i}"   &&   rm "$${i}" ; \
        done ;


