#include <stdio.h>
#include <stdlib.h>

void inc_number(int number[]);

int main(int argc, char *argv[])
{
	int number[4] = {0,0,2,1};

	int value = number[0]*1000 + number[1]*100
		+ number[2]*10 + number[3];

	printf("User input: %d\n", value);

	//printf("1: %s\n",(**char)1);

	// Think about using weighting factors?
	//int W[4] = {1000,100,10,1};

	inc_number(number);
	return 0;
}


void inc_number(int number[])
{
	int value = number[0]*1000 + number[1]*100
		+ number[2]*10 + number[3];

	printf("input = %d\n", value);

	value++;

	int output[] = {0,0,0,0};

	int digit = 0;

	for(int i=3; value > 0; i--) {
		digit = value % 10;
		output[i] = digit;
		value /= 10;
	}
	
	printf("output = %d%d%d%d\n", output[0], output[1],
			output[2], output[3]);
}
