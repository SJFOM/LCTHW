#!/bin/bash

# Abort the whole program if there's any errors
set -e

# Set up the program variables
PROGRAM=./increment
MAX_INPUT=9998

# Do a bit of cleaning up
rm -rf $PROGRAM
CFLAGS="Wall -g" make $PROGRAM

for i in `seq 0 $MAX_INPUT`;
do
	$PROGRAM i
done
