#!/bin/bash

clear

# Abort the whole program if there's any errors
set -e

# Set up the program variables
PROGRAM=./inc_number
MIN_INPUT=-9999
MAX_INPUT=9998


# Negative Loop

# for i in {-2..0};
# do

# 	a1=$(($[ $i - 1] % 10))
# 	b1=$(($[ $a1 / 10 ] % 10))
# 	c1=$(($[ $b1 / 10] % 10))
# 	d1=$(($[ $c1 / 10 ] % 10))

# 	# echo "$d1 $c1 $b1 $a1"

# 	A="$($PROGRAM $d1 $c1 $b1 $a1)"

# 	echo "$A"

# 	a2=$(($[ $i ] % 10))
# 	b2=$(($[ $a2 / 10 ] % 10))
# 	c2=$(($[ $b2 / 10 ] % 10))
# 	d2=$(($[ $c2 / 10 ] % 10))

# 	# echo "$d2 $c2 $b2 $a2"

# 	B="$($PROGRAM $d2 $c2 $b2 $a2)"

# 	echo "$B"


# 	# A="$($PROGRAM $[$i-1])"
# 	# B="$($PROGRAM $i)"
# 	if [ $[B - A] != "1" ]; then
# 		echo "ERROR: Difference is incorrect."
# 		echo "$[B - A]"
# 	fi
# done



# Positive Loop

for i in {1..30};
do

	a1=$(($[$i-1] % 10))
	b1=$(($[$a1/10]%10))
	c1=$(($[$b1/10]%10))
	d1=$(($[$c1/10]%10))

	# echo "$d1 $c1 $b1 $a1"

	A="$($PROGRAM $d1 $c1 $b1 $a1)"

	echo "$A"

	a2=$(($[$i] % 10))
	b2=$(($[$a2/10]%10))
	c2=$(($[$b2/10]%10))
	d2=$(($[$c2/10]%10))

	# echo "$d2 $c2 $b2 $a2"

	B="$($PROGRAM $d2 $c2 $b2 $a2)"


	# A="$($PROGRAM $[$i-1])"
	# B="$($PROGRAM $i)"
	if [ $[B - A] != "1" ]; then
		echo "ERROR: Difference is incorrect."
		echo "$[B - A]"
	fi
done


