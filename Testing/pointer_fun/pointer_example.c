#include <stdio.h>

int main(int argc, char *argv[])
{

	int ages[] = {10, 12, 14, 16, 18};

	int *age_pos = ages;

	printf("age_pos: %d\n", *age_pos);
	printf("Now changing the first value of ages through pointers...\n");
	*age_pos += 10;

	printf("New ages[0]: %d\n", *age_pos);

	// simply a single character named letter
	char letter = 'n';

	printf("name: %c\n", letter);

	char *name = "Friendo";

	printf("name: %c %c %c %c %c %c %c %c\n", 
		name[0], name[1], name[2], name[3], name[4], name[5], name[6], name[7]);


	printf("name: %c %c %c %c %c %c %c %c\n",
		 *name, *(name+1), *(name+2), *(name+3), *(name+4), *(name+5), *(name+6), *(name+7));

	printf("name: %s\n", name);


	/*
	 names[] is an array ([]) of characters - a string!
	 *names[] is a pointer to these strings.
	 i.e.: it points to the locations of all the strings! */
	char *names[] = {
		"Fred", "Joe",
		"Sam", "Bill", "Will"
	};

	// following this, a pointer to a series of string locations
	// is an array - an array of a collection of strings pointed 
	// to initially by names[]
	char **names_cur = names;

	printf("names_cur[0]: %s\n*names_cur: %s\n", names_cur[0], *names_cur);
	//printf("names_cur: %.s\n", names_cur);

	
	char message[] = "this is a message";
	char *massage = "but this is a massage";

	puts(message);
	puts(massage);

	printf("message[0]: %c\n", message[0]);
	printf("*(massage+1): %c\n", *(massage+1));
	printf("1[massage]: %c\n", 1[massage]);

	return 0;
}
