#!/bin/bash

# Sample code using Valgrind for performance tuning

# clear the terminal window
clear

# Re-compile to most current binary
make clean
make

# Do a bit of cleaning up
rm -r callgrind.out.* cachegrind.out.*

# Generate a callgrind.out file for the C binary given a sample input.
valgrind --dsymutil=yes --tool=callgrind ./inc_number 0 0 0 -4

# Display the C code alongside the required number of instructions (Ir).
callgrind_annotate callgrind.out.* inc_number.c

# Generate a cachegrind.out file for the C binary given a sample input.
valgrind --tool=cachegrind ./inc_number 0 0 0 -4

# Displays the number of memory read and writes (Dr and Dw).
cg_annotate --show=Dr,Dw cachegrind.out.* | grep -v "???"