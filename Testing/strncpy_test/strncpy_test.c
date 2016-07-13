#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	
	char s1[] = {'H','e','l','l','o','w','o','r','l','d'};

	int s1_count = sizeof(s1)/sizeof(char);

	s1[s1_count - 1] = '\0';
	//*s1 += '\0';

	printf("s1: %s, length: %d\n", s1, s1_count);	

	char *s2 = "Hello";

	int s2_count = sizeof(s2)/sizeof(char);

	printf("s2: %s, length: %d\n", s2, s2_count);		
	
	printf("copying (at most) %d letters from string %s into %s\n", 
			s2_count, s1, s2);

	char *res = strncpy(s2, s1, s2_count);
	if(!res) puts("string copy failed");
	s2[s2_count] = '\0';
	//printf("s1: %s\ns2: %s\n", s1, s2);

	return 0;
}
