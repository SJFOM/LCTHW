#!/bin/bash

# Abort the whole program if there's any errors
set -e

# Set up the program variables
PROGRAM=./inc_number
MAX_INPUT=9998

# Do a bit of cleaning up
#rm -rf $PROGRAM
#CFLAGS="Wall -g" make $PROGRAM

for i in `seq 2 $MAX_INPUT`;
do
	A="$($PROGRAM $[$i-1] | less)"
	B="$($PROGRAM $i | less)"
	if [ $[B - A] != "1" ]; then
		echo "ERROR: Difference is incorrect."
		echo "$[B - A]"
	fi
done

# $PROGRAM "$($PROGRAM 0001 | less)"
# $PROGRAM "$($PROGRAM 0002 | less)"

