#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{

	printf("address of argv: %p\n", &argv);
	
	/* Depending on your PC architecture, variables will have different byte
	size allocation.

	E.g.: For a pointer - 16bit PC: 2 bytes
			      32bit PC: 4 bytes
			      64bit PC: 8 bytes.

	Recall that there are 8 bits in a byte. */


	/* Note: int max_int = (int) pow(2, 8*sizeof(int))-1; gives a different
	result as pow(2, 8*sizeof(int)) must be stored temporarily as an integer
	but is cast to an integer type with the max allowable value. Then it does -1
	from this value to give MAX_INT - 1, not MAX_INT as desired.

	However, by putting the entire operation in parentheses and THEN casting to
	an integer, the correct MAX_INT can be found as you would expect it to work.*/

	int max_int = (int) (pow(2, 8*sizeof(int))-1);


	printf("Value of max signed integer representable on this machine: %d\n", max_int);
	
	printf("Max int: %d\n", max_int);

	printf("Max int + 1: %d\n", max_int+1);

	printf("Max int + 2: %d\n", max_int+2);
	
	puts("NOTE: Integer overflow");


	int unsigned max_unsigned_int = (unsigned int) pow(2, 8*sizeof(unsigned int));

	// %u is the 'unsigned' integer placeholder for the printf function.
	printf("Value of max unsigned integer representable on this machine: %u\n", max_unsigned_int);
	
	printf("Max int: %u\n", max_unsigned_int);

	printf("Max int + 1: %u\n", max_unsigned_int+1);

	printf("Max int + 2: %u\n", max_unsigned_int+2);
	
	puts("NOTE: Unsigned integer overflow");


	printf("sizeof(&argv): %ld bytes\n", sizeof(&argv));

	printf("sizeof(char): %ld byte\n", sizeof(char));
	
	printf("sizeof(int): %ld bytes\n", sizeof(int));

	printf("sizeof(unsigned int): %u bytes\n", (unsigned int)sizeof(unsigned int));

	printf("sizeof(double): %ld bytes\n", sizeof(double));
	
	printf("sizeof(long): %ld bytes\n", sizeof(long));

	printf("sizeof(float): %ld bytes\n", sizeof(float));
	
	puts("There are 8 bits in a byte");

	printf("sizeof(NULL): %ld\n", sizeof(NULL));
	puts("Why is NULL 8 bytes long?...");
	printf("NULL is a pointer to the address of 0!\nIt resides permanently at address: %p\n", NULL);

	return 0;
}
