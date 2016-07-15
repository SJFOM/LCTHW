#include <stdio.h>

void printBits(size_t const size, void const * const ptr);

int main(int argc, char **argv){

	int x, number, bit, n = 0;

	if(argc > 1){
		number = atoi(argv[1]);
		x = atoi(argv[2]);
	} else{
		printf("Please enter the number and bit to toggle\n");
		number = 0;
		x = 0;
	}

	printf("Number: %d\t=>\t", number);
	printBits(sizeof(number), &number);
	printf("Bit to toggle: %d\n", x+1);
	printf("\n/*----------------------------------------*/\n\n");

	/*----------------------------------------*/

	// Bitwise operations:

	number |= 1 << x;
	printf("Setting bit %d: ", x+1);
	printBits(sizeof(number), &number);
	printf("\n/*----------------------------------------*/\n\n");

	/*----------------------------------------*/

	number &= ~(1 << x);
	printf("Clearing bit %d: ", x+1);
	printBits(sizeof(number), &number);
	printf("\n/*----------------------------------------*/\n\n");

	/*----------------------------------------*/

	number ^= 1 << x;
	printf("Toggling bit %d: ", x+1);
	printBits(sizeof(number), &number);
	printf("\n/*----------------------------------------*/\n\n");

	/*----------------------------------------*/

	bit = (number >> x) & 1;
	printf("Checking bit %d: %d\n", x+1, bit);
	printf("\n/*----------------------------------------*/\n\n");

	/*----------------------------------------*/

	x = 0;
	number ^= (-x ^ number) & (1 << n);
	printf("Changing bit number %d to %d: ", n+1, x);
	printBits(sizeof(number), &number);
	printf("\n/*----------------------------------------*/\n\n");

	/*----------------------------------------*/


	return 0;
}

// String to binary - assumes little endian
// see: http://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}