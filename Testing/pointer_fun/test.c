#include <stdio.h>

int main(int argc, char *argv[])
{
	char message[] = "this is a message";
	char *massage = "but this is a massage";

	puts(message);
	puts(massage);

	printf("message[0]: %c\n", message[0]);
	printf("*(massage+1): %c\n", *(massage+1));
	printf("1[massage]: %c\n", 1[massage]);

	return 0;
}
