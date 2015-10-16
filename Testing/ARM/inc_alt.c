#include <stdio.h>

int *inc_number(int number[]);

int main(int argc, char *argv[])
{
	int number[4] = {1,2,3,4};

	//int value = number[0]*1000 + number[1]*100
	//	+ number[2]*10 + number[3];

	//printf("User input: %d\n", value);

	int *back = inc_number(number);

	printf("back: %d,%d,%d,%d\n", back[0], 
		back[1], back[2], back[3]);
	return 0;
}


int *inc_number(int number[])
{
	register unsigned int value = number[0]*1000 + number[1]*100
		+ number[2]*10 + number[3] + 1;

	if(number[3] == 9){
		if(number[2] == 9){
			if(number[1] == 9){
				if(number[0] == 9){
						number[] = {9,9,9,9};
				}
			}
		}
	}

	//printf("output = %d%d%d%d\n", number[0], number[1],
	//		number[2], number[3]);
	
	return number;
}
