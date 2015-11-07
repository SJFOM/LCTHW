#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// stdlib.h required for use of the atoi function call.
// string.h required for use of memset function call.

// A prototype of the function so it is recognised in main().
// Alternatively, place main() at bottom of code instead.
int *inc_number(int number[]);


int main(int argc, char *argv[])
{
	// Initialises the number[] array where each element in
	// the array is taken as an input argument by the user.
	// argv[] is a pointer to an array of characters (string), the values
	// of which are received from the user in the terminal window:
	// Example: "./inc_number 9 6 7 4" gives argv[1] = 9, argv[2] = 6 etc..
	// The atoi function converts the initial portion of the string to 
	// an integer.
	int number[4] = {atoi(argv[1]),
	atoi(argv[2]),
	atoi(argv[3]),
	atoi(argv[4])};

	// Calls upon the incrementing function, passing the number[] array
	// as it's argument - the array to be incremented.
	// Note: *output is a pointer to an integer - here it is assigned
	// the base address of result produced from the inc_number() function.
	// The base address of *output can be incremented like a regular array.
	int *output = inc_number(number);

	// Prints the resulting output to the terminal window.
	printf("%d%d%d%d\n", output[0], 
		output[1], output[2], output[3]);

	// Report to main() that the program has executed correctly.
	return 0;
}

// The incrementing function, taking an array representation of a number
// as its input and returning the incremented result as a pointer to an
// integer (i.e: in the corresponding array format).
int *inc_number(int number[])
{	
	// Declaring function variables.
	// The register keyword can flag to some compilers to place this 
	// value in a local register thus providing faster access.
	// Knowing that a value will never be negative allows it to be assigned
	// as "unsigned" as some processors can handle unsigned integers considerably
	// faster than signed ones.
	register unsigned int i = 0;
	register unsigned int sign = 0;


	// This for-loop checks for any negative values in the array input.
	// E.g.: [0,-4,5,2] => -452
	// It is designed to check for negative values from most signficant bit (MSB)
	// to least significant bit (LSB) as there are more possible input values
	// falling into this category than that of the LSB being negative.
	// The for loop contains an if statement which flags if a negative value is
	// found and temporarily changes it to be positive for ease of computation 
	// when calculating the "value" below.
	// The loop is designed to break once a negative value is found as there
	// should only be one.
	for(i = 0; i <= 3; i++) {
		if(number[i] < 0){
			number[i] *= -1;
			sign = 1;
			break;
		}
	}

	// The integer value of the array input is found (as a positive number
	// for now) and is weighted appropriately.
	register unsigned int value = number[0]*1000
	+ number[1]*100
	+ number[2]*10
	+ number[3];

	// This concise ternary operator performs the main "incrementing" 
	// functionality of this function. A check must be performed to 
	// ensure that the code is incrementing in the right direction, 
	// i.e. towards zero if negative, away/above zero if positive.
	// The ternary operator is similar to an if-else statement where:
	// if(sign == 1){value += -1} else { value += 1}
	value += (sign) ? -1 : 1;

	// A fast C implementation of setting each element in "number" to zero.
	// E.g.: memset(myArray, value_to_be_set, #of_elements*sizeof(element));
	// This is required as the next for-loop that assigns new incremented 
	// values to "number" is not guaranteed to access all elements depending
	// on the number being incremented.
	memset(number, 0, 4 * sizeof(number[0]));

	// Sets the constant value of the divisor used in the upcoming for-loop.
	register unsigned int div = 10;

	// This for-loop assigns the new incremented value to the "number" array.
	// The loop functionality performs modulo arithmetic to reduce "value" 
	// to it's corresponding array elements (where modulo is the remainder
	// of a division operation).
	// The (i!= -1) condition is required for "value"'s that would cause
	// "i" to become out of bounds as an index (e.g.: value=9999).
	// A decrementing loop is chosen as it is commonly faster to process "i--"
	// as the test condition.
	// Aside: if value=9999 the code is designed to overflow to 0000 rather than
	// crash.
	for(i=3; (value > 0) && (i != -1); i--) {
		number[i] = value % div; 
		value /= div;
	}

	// The correct polarity of the number is re-assigned at the correct index.
	if(sign){number[i+1] *= -1;}

	// return the result of this function to the caller.
	return number;
}
