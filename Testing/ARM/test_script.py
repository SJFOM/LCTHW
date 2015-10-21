#!/usr/bin/env python

# subprocess is required to interface with the terminal window.
# sys is required to create an exit call if there is an error.
import subprocess
import sys

# Set up the program variables
PROGRAM = './inc_number'
MIN_INPUT=-9999
MAX_INPUT=9999
indx = 0
num = [0] * 4

# This for-loop iterates over the total allowable range of a
# 4-element array and tests the functionality of "inc_number.c"
# for each value. The code also checks the output of ./inc_number
# for a given input number to see if it has been incremented by
# 1 only, otherwise the user is alerted to an error.

for i in range(MIN_INPUT, MAX_INPUT+1):

    # "j" is set to the value of "i" to retain its original value
    # as "i"'s polarity may be changed later on.
    j = i

    # The value of "i" is converted to positive - must use absolute as
    # "i" might = 0.
    if i < 0:
        i = abs(i)

    # A useful python function to break up the input number into its
    # constituent list (array) parts.
    temp = list(str(i))

    # Zero padding is needed due to the nature of the C-code's functionality.
    num = [0]*(4-len(temp))
    # The remaining entries in "num" are added making a list of size 4.
    num.extend(temp)

    # num is currently in string format and needs to be converted (mapped)
    # to a list of integer values for the next step which re-assigns
    # polarity if the "i" was originally negative.
    num = map(int, num)

    # if "i" was originally negative then its polarity needs to be
    # re-assigned in list form.
    # "indx" is found as the first non-zero element in the "num" list.
    # "next" runs the generator "i for i, x in enumerate(num) if x" until
    # this condition is met and the correct index (indx) is found.
    # "i for i" keeps the loop going and "enumerate(num)" finds the index
    # value of each element in "num". "if x" checks if x != 0.
    # if no non-zero value is found, "-1" is assigned to "indx" as it will
    # access the last value in the list.
    if j < 0:
        indx = next((i for i, x in enumerate(num) if x), -1)
        num[indx] *= -1

    # Interfacing commands with the terminal requires str formatted commands
    # to be used - remaps "num" to str.
    num = map(str, num)

    # The command to be sent to the terminal.
    cmd = [PROGRAM, num[0], num[1], num[2], num[3]]

    # Communicates with the terminal providing "cmd" and assigning the output
    # of the ./inc_number code to a list named "output"
    output = list(subprocess.Popen(cmd, stdout=subprocess.PIPE, 
        stderr=subprocess.PIPE).communicate()[0])


    # "output" is in string list form and can't be directly converted to an
    # int if it contains a "-" symbol. This if-else statment removes it if
    # it exists, sets the correct polarity and converts to a number.
    if '-' in output: 
        output.remove('-')
        output = int(''.join(map(str,output)))*-1
    else:
        output = int(''.join(map(str,output)))

    print "input: ", j

    print "output: ", output, "\n"

    # Code to check that output minus input is always 1 as required.
    # Program is exited if this condition is not met or the program finishes.
    if output - j != 1:
        print "*"*60
        print "\nDifference = ", "(",output,") - (", j, ")",
        ", should be 1."
        sys.exit(" Either integer difference is incorrect OR "
            "end of loop encountered.")


