#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	/* atoi() converts a string to an integer.
	atoi is commonly used to take a number "x"
	in string format and return its integer value.
	useful when passing argv terms. */

	if(argv[1]){
		 printf("first argument as integer value: %d\n",
			atoi(argv[1]));
		printf("value doubled: %d\n", atoi(argv[1])*2);
	}

	return 0;
}
