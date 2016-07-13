#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 0;
	
	// go through each string in argv
	// why am I skipping argv[0]?
	/* ANS: argv[0] is "./ex10" as you call it 
	   in the terminal. "./ex10 i am Sam" in the 
	   terminal will return "i am Sam" as the first
	   three arguments */

	for(i = 1; i < argc; i++) {
		printf("arg %d: %s\n", i, argv[i]);
	}

	// let's make our own array of strings
	char *states[] = {
		"California", "Oregon",
		"Washington", "Texas"
	};
	int num_states = 4;

	for(i = 0; i < num_states; i++) {
		printf("state %d: %s\n", i, states[i]);
	}


	for(i = 0; i < 10; i++) { 
		printf("first state index: %i, character: %c\n", i, states[0][i]);
	}

	return 0;
}
