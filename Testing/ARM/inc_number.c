#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *inc_number(int number[]);

int main(int argc, char *argv[])
{
	int number[4] = {atoi(argv[1]),
		atoi(argv[2]),
		atoi(argv[3]),
		atoi(argv[4])};

		int *output = inc_number(number);

		printf("%d%d%d%d\n", output[0], 
			output[1], output[2], output[3]);

		return 0;
	}


int *inc_number(int number[])
{	

	register unsigned int i = 0;
	register unsigned int sign = 0;
	unsigned int flag[4] = {1000,100,10,1};

	for(i = 0; i <= 3; i++) {
		if(number[i] < 0){
			number[i] *= -1;
			sign = 1;
			break;
		}
	}

	register unsigned int value = number[0]*flag[0]
	+ number[1]*flag[1]
	+ number[2]*flag[2]
	+ number[3]*flag[3];

	value += (sign) ? -1 : 1;

	memset(number, 0, 4 * sizeof(number[0]));

	register unsigned int div = 10;

	for(i=3; (value > 0) && (i != -1); i--) {
		number[i] = value % div; 
		value /= div;
	}

	if(sign){number[i+1] *= -1;}

	return number;
}
