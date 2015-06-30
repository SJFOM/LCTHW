#include <stdio.h>

// Provides a macro called assert which can be used to verify 
// assumptions made by the program and print a diagnostic message
// if this assumption is false.
#include <assert.h>

// Needed for memory functions: malloc and free.
#include <stdlib.h>

// Required for function strdup.
#include <string.h>



// Must first initialise the struct and the types it uses
struct Person {
	char *name;
	int age;
	int height;
	int weight;
};


/* The purpose of this function is to instantiate an occurence of the
"Person" compound type. Think of it as filling in the gaps for the 
initial Person struct declared above. */
struct Person *Person_create(char *name, int age, int height, int weight)
{

	/* This pointer points at a block of memory that is just the right 
	size to store this compund struct type "Person". malloc, or memory
	allocate, is used to create this space in memory. */
	struct Person *who = malloc(sizeof(struct Person));

	/* This function is imported via <assert.h> and is used to determine
	whether the given statement is true or false. Assert is generally 
	used to check for memory allocation and in this instance will check
	that malloc has returned a valid pointer. An unset or invalid pointer
	would have the value "NULL" (a special character) - the value this 
	assert function is checking for. If the assert function finds the 
	statement to be true, then it will automatically abort the program */

	// i.e.: "assert" that who is not equal to NULL
	assert(who != NULL);

	/* strdup duplicates the string for the name so struct definitely
	owns it. strdup is actually like malloc as well as it and it also
	copies the original string into the new memory it creates. This is
	more than likely because char *name is of an undefined size (name
	sizes vary from person to person) and so the right amount of memory
	can only be decided at time of memory allocation. */
	who->name = strdup(name);
	who->age = age;
	who->height = height;
	who->weight = weight;


	// return the pointer to the block of memory containing this new
	// compund type.
	return who;
}


/* This function is needed because a Person_create exists. It is 
essential that allocated memory also be freed once a program has
completed its life-cycle. This is to avoid any "memory leaks". */
void Person_destroy(struct Person *who)
{
	/* Same task as before. Imoprtant check required at both memory
	allocation freeing stages as struct data can be changed inbetween
	both stages. */
	assert(who != NULL);
	
	// Free the memory you were given through malloc and strdup.
	free(who->name);
	free(who);
}

// Simple function used to print each aspect of a "Person" struct.
void Person_print(struct Person *who)
{
	// Note the use of who->___ for getting the "field" from the struct.
	printf("Name: %s\n", who->name);
	printf("\tAge: %d\n", who->age);
	printf("\tHeight: %d\n", who->height);
	printf("\tWeight: %d\n", who->weight);

	// Type is void - no return.
}

int main(int argc, char *argv[])
{
	// make two people structures
	struct Person *joe = Person_create(
		"Joe Alex", 32, 64, 140);

	struct Person *frank = Person_create(
		"Frank Blank", 20, 72, 180);

	// print them out and where they are in memory
	printf("Joe is at memory location: %p\n", joe);
	Person_print(joe);
 
	printf("Frank is at memory location: %p\n", frank);
	Person_print(frank);

	// make everyone age by 20 years and print them again

	printf("Aging everyone by 20 years...\n");

	joe->age += 20;
	joe->height -= 2;
	joe->weight += 40;
	Person_print(joe);

	frank->age += 20;
	frank->weight += 20;
	Person_print(frank);

	// destroy them both so we clean up
	Person_destroy(joe);
	Person_destroy(frank);

	return 0;
}


