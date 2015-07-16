#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct Person {
	char *name;
	char initial;
	int age;
	int height;
	int weight;
};

struct Person *Person_create(char *name,char initial, int age, int height, int weight)
{
	struct Person *who = malloc(sizeof(struct Person));

	assert(who != NULL);

	who->name = strdup(name);
	who->initial = initial;
	who->age = age;
	who->height = height;
	who->weight = weight;

	return who;
}

void Person_print(struct Person *who)
{
	printf("%s .%c is: %d years old\n%d cm tall\n%d kgs\n", 
		who->name, who->initial, who->age, who->height, who->weight);
}

void Person_destroy(struct Person *who)
{
	assert(who != NULL);

	free(who->name);
	free(who);
}


int main(int argc, char *argv[])
{
	puts("Hello World");

	puts("Creating a person...");

	struct Person *joe = Person_create("Joe",'C', 24, 180, 80);
	
	Person_print(joe);

	Person_destroy(joe);
	return 0;
}
