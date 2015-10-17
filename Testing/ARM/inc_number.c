#include <stdio.h>

int *inc_number(int number[]);

int main(int argc, char *argv[])
{
	int number[4] = {-1,2,3,4};

	int value = number[0]*1000 + number[1]*100
		+ number[2]*10 + number[3];

	printf("User input: %d\n", value);

	int *output = inc_number(number);

	printf("output: %d,%d,%d,%d\n", output[0], 
		output[1], output[2], output[3]);
	return 0;
}


int *inc_number(int number[])
{	
	register unsigned int i = 0;
	register unsigned int sign = 0;
	int flag[4] = {1000,100,10,1};
	
	for(i = 0; i <= 3; i++) {
		if(number[i] < 0){
			flag[i] *= -1;
			sign = 1;
			break;
		}
	}

	// printf("flag = %d,%d,%d,%d\n", flag[0], flag[1],
	// 		flag[2], flag[3]);


	// Maybe try using the indexing of the previous for loop for the value and just += it as a sum?
	//if(value > 9999) { value /= 10;}

	register unsigned int value = number[0]*flag[0]
	+ number[1]*flag[1]
	+ number[2]*flag[2]
	+ number[3]*flag[3];

	if(sign == 0){
		value++;
	} else {
		puts("Yep, I'm negative");
		value--;
		//value *= -1;
		number[i] *= -1;
	}

	register unsigned int temp = i;

	register unsigned int div = 10;

	for(i=3; value > 0; i--) {
		number[i] = value % div; 
		value /= div;
	}

	number[temp] *= -1;

	return number;
}
