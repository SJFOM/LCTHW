/* This program should have the same functionality as ex16.c but 
without the use of pointers or malloc. */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
	char *name;
	int age;
	int height;
	int weight;
};


void Person_print(struct Person who)
{
	printf("Name: %s\n", who.name);
	printf("\tAge: %d\n", who.age);
	printf("\tHeight: %d\n", who.height);
	printf("\tWeight: %d\n", who.weight);

}

int main(int argc, char *argv[])
{	

	struct Person joe = {"Joe Alex",
		32, 64, 140};

	printf("%s's age: %d\n", joe.name, joe.age);

	struct Person frank = {"Frank Blank",
		20, 72, 180};

	printf("%s's age: %d\n", frank.name, frank.age);
	
	// print them out and where they are in memory
	printf("Joe is at memory location: %p\n", &joe);
	Person_print(joe);

	printf("Frank is at memory location: %p\n", &frank);
	Person_print(frank);

	// make everyone age by 20 years and print them again

	printf("Aging everyone by 20 years...\n");

	joe.age += 20;
	joe.height -= 2;
	joe.weight += 40;
	Person_print(joe);

	frank.age += 20;
	frank.weight += 20;
	Person_print(frank);

	//free(joe.name); - required if strdup is used for name, 
	// otherwise valgrind reports a memory leak.
	return 0;
}
