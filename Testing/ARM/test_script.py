#!/usr/bin/env python
import subprocess
import sys

# Set up the program variables
PROGRAM = './inc_number'
MIN_INPUT=-9999
MAX_INPUT=9999
indx = 0
num = [0] * 4

# Iterative Loop

for i in range(MIN_INPUT, MAX_INPUT+1):

    j = i
    sign = 0

    if i < 0:
        sign = 1;
        i = abs(i)

    temp = list(str(i))

    num = [0]*(4-len(temp))
    num.extend(temp)

    num = map(int, num)

    if sign == 1:
        indx = next((i for i, x in enumerate(num) if x), -1)
        num[indx] *= -1

    num = map(str, num)

    cmd = [PROGRAM, num[0], num[1], num[2], num[3]]

    output = list(subprocess.Popen(cmd, stdout=subprocess.PIPE, 
        stderr=subprocess.PIPE).communicate()[0])

    if '-' in output: 
        output.remove('-')
        output = int(''.join(map(str,output)))*-1
    else:
        output = int(''.join(map(str,output)))

    print "input: ", j

    print "output: ", output, "\n"

    if output - j != 1:
        print "*"*60
        print "\nDifference = ", "(",output,") - (", j, ")",
        ", should be 1."
        sys.exit(" Either integer difference is incorrect OR "
            "end of loop encountered.")


