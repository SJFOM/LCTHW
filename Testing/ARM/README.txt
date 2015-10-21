**************************
INCREMENTING AN ARRAY

Written by Sam O'Mahony

samjfom@gmail.com
**************************

inc_number.c contains the relevant inc_number() function in C code. The inc_number() function is designed to be efficient. It does not contain any error flagging as it was decided that the code was intended to be purely functional and thus minimal. Any error catching should be done before calling the function or by the user in its implementation. 

The function is capable of taking both negative and positive "array numbers" over the max allowable range [-9,9,9,9 to +9,9,9,9]. Upon receiving 9999, the code will overflow to 0000 as a safe value.

The code is designed to be accesed from the terminal via:
"./inc_number number[0] number[1] number[2] number[3]" where number[] is 
an integer array of length 4. 
	e.g.: ./inc_number 1 2 3 4
		... 1235     <= terminal output
	
Negative numbers are represented in the form [0,-4,4,3] => -443.
	e.g.: ./inc_number 0 -4 4 3
		... 0-442     <= terminal output

Compile the C code using "make" (Makefile provided). Clean using "make clean"

A test script is provided in Python (test_script.py). This code cycles through all possible input values and checks the C-code operation for each input value. Execute using "time python test_script.py" for extra information on time elapsed.

If the user has Valgrind installed, the executable shell script "grind_check.sh" may be useful. It provides some form of performance evaluation by giving instruction counts, memory read counts and memory write counts for the code provided. It also displays the C-code alongside a numeric tally for each line of code describing the amount of instructions required for each line of code. See output of "callgrind_annotate callgrind.out.* inc_number.c" for these results.
