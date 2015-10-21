#!/bin/bash

clear

# Abort the whole program if there's any errors
set -e

# rm -r callgrind.out* cachegrind.out*

valgrind --dsymutil=yes --tool=callgrind ./inc_number 0 0 0 -4

callgrind_annotate callgrind.out.* inc_number.c

