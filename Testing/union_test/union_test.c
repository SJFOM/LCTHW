/*
* Example code to demonstrate the use of the union identifier
*
* For the following type:

struct strt
{
   float f;
   char c;
   int a;
}

*
*	You will get the classic struct data packing structure
*

|0---1---2---3---|4---|5---6---7---8---|
|ffffffffffffffff|    |                | <- f: Where your float is stored
|                |cccc|                | <- c: Where your char is stored
|                |    |aaaaaaaaaaaaaaaa| <- a: Where your int is stored


*
*	However, for the data type:
*

union unin
{
   float f;
   char c;
   int a;
}

*	You will get the following data packing structure
*
*	

|0---1---2---3---|
|ffffffffffffffff| <- f: where your float is stored
|cccc------------| <- c: where your char is stored
|aaaaaaaaaaaaaaaa| <- a: where your int is stored

*
*
* See: http://stackoverflow.com/questions/724261/unions-versus-structures-in-c
*
*/



#include <stdio.h>
#include <string.h>
 
// union {
// OR
// union Data_union
// OR
typedef union Data_Union {

	unsigned char ByteArray[10];

	struct{
		unsigned char a;
   		unsigned char b;
   		unsigned char c;
	};

	struct{
		unsigned char d;
   		unsigned char e;
   		unsigned char f;
	};

	struct{
		unsigned char g;
   		unsigned char h;
   		unsigned char i;
   		unsigned char j;
	}THE_REST;
} DATA_UNION;
   
 
int main( ) {    

   // union Data_Union du;
   // OR, if typedef union is used
   DATA_UNION du;

   du.ByteArray[0] = 0;
   du.ByteArray[1] = 1;
   du.ByteArray[2] = 2;

   du.ByteArray[3] = 3;
   du.ByteArray[4] = 4;
   du.ByteArray[5] = 5;

   du.ByteArray[6] = 6;
   du.THE_REST.h = 7;
   du.ByteArray[8] = 8;
   du.THE_REST.j = 9;


   printf( "du.d : %d\n", du.d);

   printf( "Memory size occupied by data : %lu\n", sizeof(du));

   return 0;
}
