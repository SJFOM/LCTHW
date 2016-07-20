#!/bin/bash

# Abort the whole program if there's any errors
set -e

# Set up the program variables
MAX_ROWS=10
PROGRAM=./ex17_extra_credit
DB=new.dat

# do a bit of cleaning up
rm -rf $PROGRAM $PROGRAM.d*
CFLAGS="-Wall -g" make $PROGRAM
rm -f $DB

# Create a new database
$PROGRAM $DB c 50 $MAX_ROWS


echo "id age name email"
# simple for loop to fill the new database
for i in `seq 0 $[$MAX_ROWS-1]`;
do
	$PROGRAM $DB s $i $i name$i email$i
done

# list the database entries
output=$($PROGRAM new.dat l)
$PROGRAM $DB l

output=$($PROGRAM $DB g 4)
echo $output
$PROGRAM $DB g 4


echo $($PROGRAM $DB f name name4)
$PROGRAM $DB f name name4

$PROGRAM $DB f email email6

$PROGRAM $DB f age 9



