#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
	char *name;    // 8 bytes: 8
	char initial;  // 1 bytes: 9
	int age:6;     // 6 bits: 9 bytes and 6 bits
	int height:10; // 10 bits: 9 bytes and 16 bits
	int weight:10; // 10 bits: 9 bytes and 26 bits
		       // Remember, 8 bits in a byte
		       // --> 12 bytes and 3 bits
	// need to pad the remaining 4 bytes as Person is 8 byte self aligned.
	// pad with 5 bits and 3 bytes --> total = 16 bytes	
};

struct Person *Person_create(char *name, char initial, int age, int height, int weight)
{
	struct Person *who = malloc(sizeof(struct Person));

	if(!who) {
		puts("couldn't alloc memory for a Person struct");	
		exit(1); }

	who->name = strdup(name);
	who->initial = initial;
	who->age = age;
	who->height = height;
	who->weight = weight;

	return who;
}

int main(int argc, char *argv[])
{
	
	struct Person *John = Person_create("John", 'C',
			24, 180, 80);

	printf("%s %c is %d years old, is %d cm tall and weight %d kgs\n",
		John->name, John->initial, John->age, John->height,
					John->weight);

	printf("A struct has:\nchar*: %zu bytes\nchar: %zu bytes\n",
		sizeof(char*), sizeof(char));
	
	/* can't do this - sizeof cannot return size of bit-field,
	only bytes!
	printf("int:6: %zu\nint:10*2: %zu\n",
		sizeof(John->age), sizeof(John->height)*2); */

	printf("Size of struct: %zu bytes\n", sizeof(struct Person));

	free(John->name);
	free(John);

	return 0;
}
