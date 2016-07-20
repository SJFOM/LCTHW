#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *inc_number(int *number, int length);


int main(int argc, char *argv[])
{
	int number[4] = {argv[1][0] - '0',
	argv[2][0] - '0',
	argv[3][0] - '0',
	argv[4][0] - '0'};

	int *output = inc_number(number, 4);

	printf("%d,%d,%d,%d\n", output[0], 
	output[1], output[2], output[3]);

	return 0;
}

int *inc_number(int *number, int length)
{	
	
	unsigned int i = 0;
	unsigned int FLAG = 0;

	for(i = length-1; ; i--){
		printf("number[%d] = %d\n", i, number[i]);
		if(number[i] != 9){
			number[i]++;
			break;
		} else{
			number[i] = 0;
			FLAG = (i=length) ? 1 : 0;
			printf("FLAG = %d\n", FLAG);
		}
	}

	return number;

}
