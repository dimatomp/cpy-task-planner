#!/bin/sh

clang++ main.cc ../taskplanner.cc -o main -I$LEMON_INCLUDE_PATH --std=c++17