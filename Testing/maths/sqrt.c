#include <stdio.h>
#include <math.h>

int main(int argc, char **argv){

	printf("Value is: %s\n", argv[1]);
	printf("It's square root: %.3d\n", sqrt((double)argv[1]));
	return 0;
}
