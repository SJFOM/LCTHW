#include <stdio.h>

int main(int argc, char *argv[])
{

	int ages[] = {10, 12, 14, 16, 18};

	int *age_pos = ages;

	printf("age_pos: %d\n", *age_pos);
	printf("Now changing the first value of ages through pointers...\n");
	*age_pos += 10;

	printf("New ages[0]: %d\n", *age_pos);



	return 0;
}