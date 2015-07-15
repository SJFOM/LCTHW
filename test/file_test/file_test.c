#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *myfile = fopen("new_database.dat", "w");
	if(!myfile) perror("Here's my message");

	// valgrind reports 1 alloc with 0 frees if fclose isn't used.
	fclose(myfile);
	return 0;
}
