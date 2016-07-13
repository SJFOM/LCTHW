#include <string.h>
int main()
{
    char *str1 = "string Literal";
    const char *str2 = "string Literal";
    char source[] = "Sample string";

    strcpy(str1,source);    //No warning or error, just Undefined Behavior
    strcpy(str2,source);    //Compiler issues a warning

    return 0;
}
