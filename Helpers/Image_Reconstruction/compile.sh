#!/usr/bin/env bash

g++ -O3 -ffast-math -funsafe-loop-optimizations --std=c++0x Simple_Backproj.cc -o simple_backproj   `Magick++-config --cppflags --ldflags --libs`  -lfftw3 -lm 
