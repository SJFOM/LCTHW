#!/usr/bin/env python

import subprocess

# Set up the program variables
PROGRAM = './inc_number'
MIN_INPUT=-9999
MAX_INPUT=9998
div=10

numA = [0] * 5
numB = numA

# Negative Loop

for i in range(7, 10):

    for j in range(3,-1,-1):
        numA[j] = abs(i-1) % div
        numB[j] = abs(i) % div
        i /= div

    
    print numA[3]
    print numB[3]

    # print [int(j) for j in str(i)]
    

    A = int(subprocess.Popen([PROGRAM, str(numA[0]), str(numA[1]),
        str(numA[2]), str(numA[3])], stdout=subprocess.PIPE, 
        stderr=subprocess.PIPE).communicate()[0])

    B = int(subprocess.Popen([PROGRAM, str(numB[0]), str(numB[1]),
        str(numB[2]), str(numB[3])], stdout=subprocess.PIPE, 
        stderr=subprocess.PIPE).communicate()[0])

    # print A
    # print B


    if(B-A != 1):
        print "ERROR: Integer difference is incorrect"
        print B-A


# print [str(j) for j in str(-100)]
# print [int(char) for char in str(634)]

# print list(str(-10))

# numA *= 0

# numA = list(str(-1))

# print numA[0], numA[1], numA[2], numA[3], numA[4]




