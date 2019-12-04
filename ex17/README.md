# *Exercise 17: Heap and Stack Memory Allocation*

## How to Break It

### The classic way is to remove some of the safety checks such that you can pass in arbitrary data. For example, if you remove the check on line 160 that prevents you from passing in any record number.
i.e. - remove the line:
```c
if(id >= MAX_ROWS) die("There's not that many records.");
```
Now, the program doesn't get to warn the user when you use an invalid ID number and you get problems like the following:

```
➜  ex17 git:(master) ✗ ./ex17 db.dat s 101 name name@zedshaw.com
➜  ex17 git:(master) ✗ ./ex17 db.dat l                          
0 zero zero@zedshaw.com
1 zed zed@zedshaw.com
2 frank frank@zedshaw.com
3 joe joe@zedshaw.com
4 sam sam@zedshaw.com
80 eighty eighty@zedshaw.com
➜  ex17 git:(master) ✗ ./ex17 db.dat g 101
ERROR: ID is not set
➜  ex17 git:(master) ✗ 
```
###You can also try corrupting the data file. Open it in any editor and change random bytes then close it.

First just copy over the original db.dat contents.

```
➜  ex17 git:(master) ✗ cat db.dat > corrupt.dat
```
Now, open it up and remove a few bytes. Try call ./ex17 using this new file and check those nasty results!

```
➜  ex17 git:(master) ✗ vim corrupt.dat       
➜  ex17 git:(master) ✗ ./ex17 corrupt.dat l  
1  @zedshaw.com
1  zedshaw.com
1 k k@zedshaw.com
1  zedshaw.com
1  zedshaw.com
1 ty ty@zedshaw.com
➜  ex17 git:(master) ✗ ./ex17 corrupt.dat g 3
1  zedshaw.com
➜  ex17 git:(master) ✗ 
```

### You could also find ways to pass bad arguments to the program when it's run, such as getting the file and action backwards will make it create a file named after the action, then do an action based on the first character.

```
➜  ex17 git:(master) ✗ ./ex17 c case.dat
➜  ex17 git:(master) ✗ ./ex17 c c
c            corrupt.dat
```

### There is a bug in this program because of strncpy being poorly designed. Go read about strncpy then try to find out what happens when the name or address you give is greater than 512 bytes. Fix this by simply forcing the last character to '\0' so that it's always set no matter what (which is what strncpy should do).

Using the following python code to create a large enough string for entry:

```py
#!/usr/bin/env python

from subprocess import call

try:
    call(['./ex17', 'db.dat', 'd', '5']);
    call(['./ex17', 'db.dat', 'd', '6']);

    # name is 494 bytes long.
    name = "abcdefghijklmnopqrstuvwxyz"*19
    arguments = ['./ex17', 'db.dat', 's', '5', name, 'zed@zedshaw.com']

    call(arguments);

    # name is now 520 bytes - 8 too many!
    name = "abcdefghijklmnopqrstuvwxyz"*20
    arguments = ['./ex17', 'db.dat', 's', '6', name, 'zed@zedshaw.com']

    call(arguments);
except Exception, e:
	print e
```

Sample output from this file shows that strncpy gives a name error for ID number 6.

```
➜  ex17 git:(master) ✗ ./ex17 db.dat l
0 zero zero@zedshaw.com
1 zed zed@zedshaw.com
2 frank frank@zedshaw.com
3 joe joe@zedshaw.com
4 sam sam@zedshaw.com
5 abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz zed@zedshaw.com
6 abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrzed@zedshaw.com zed@zedshaw.com
80 eighty eighty@zedshaw.com
```

Can fix this by appending a NULL '\0' character to the end of the addr->name and addr->email strings.

```c
void Database_set(struct Connection *conn, int id, const char *name,
	const char *email) 
{  
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete it first", conn);

	addr->set = 1;

	char *res = strncpy(addr->name, name, MAX_DATA);
	// Apply the fix.
	addr->name[MAX_DATA-1] = '\0';
	// demonstrate the strncpy bug
	if(!res) die("Name copy failed", conn);

	res = strncpy(addr->email, email, MAX_DATA);
	// Apply the fix.
	addr->email[MAX_DATA-1] = '\0';
	if(!res) die("Email copy failed", conn);
}
```

### In the extra credit I have you augment the program to create arbitrary size databases. Try to see what the biggest database is before you cause the program to die for lack of memory from malloc.

Keep increasing MAX_DATA and MAX_ROWS until your memory runs out - depends on how much RAM you have!

## Extra Credit

### The die function needs to be augmented to let you pass the conn variable so it can close it and clean up.

This is easily ammended by adding a call to Database\_clean() in the die function. Note the prototype Database\_clean function coded above die(). 

```c
void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{

	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	Database_close(conn);

	exit(1);
}
```

Every call to the die() function now takes two parameters with the second being the "conn" Connection struct created in int main. Note also the first call to die() in int main passes NULL as its Connection struct as one has not yet been instantiated in the code.

```c
int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);
	.
	.
	.
```

### Change the code to accept parameters for MAX_DATA and MAX_ROWS, store them in the Database struct, and write that to the file, thus creating a database that can be arbitrarily sized.

```
See ex17_extra_credit.c
```
### Add more operations you can do on the database, like find.

Added "find" functionality to the ex17_with_find.c program which allows the user to search for a database entry by name or email (must be specified by user). The extra action is given by 'f' and is documented in the switch case in int main as:

```c
case 'f':
		if(argc < 5) die("USAGE: ex17 <dbfile> f <name/email> \"search term\"", conn);

		const char *term = argv[3];
		const char *search = argv[4];
		if(strcmp(term, "name") == 0) {
			// search for name
			find(1, search, conn);
		} else if(strcmp(term, "email") == 0) {
			// search for email
			find(0,search, conn);
		}
		break;

```
The function "find" is as follows:

```c
void find(int state, const char *search, struct Connection *conn)
{
	int i = 0;
	int found = 0;
	
	struct Database *db = conn->db;

	for(i = 0; i < db->MAX_ROWS; i++)
	{
		struct Address *cur = db->rows[i];
		
		if((!(strcmp(search,cur->name)) && state) || (!(strcmp(search, cur->email)) && !state)){
			found = 1;
			Address_print(cur);}
	}
	if(!found) puts("entry not found");
}

```


### Read about how C does it's struct packing, and then try to see why your file is the size it is. See if you can calculate a new size after adding more fields.

Found a very good article that explains the topic well.

```url
http://www.catb.org/esr/structure-packing/
```
Created a file named struct_padding.c in the extra_credit directory to explore bit padding in structs.

### Add some more fields to the Address and make them searchable.

Added an ```c int age``` field to Address and made it searchable with the find function as described above. Minor changes include adding ```c state==2``` for the case where the search term is "age".

### Write a shell script that will do your testing automatically for you by running commands in the right order. Hint: Use set -e at the top of a bash to make it abort the whole script if any command has an error.

```sh
#!/bin/bash

# Abort the whole program if there's any errors
set -e

# Set up the program variables
MAX_ROWS=10
PROGRAM=./ex17_extra_credit
DB=new.dat

# do a bit of cleaning up
rm -rf $PROGRAM $PROGRAM.d*
CFLAGS="-Wall -g" make $PROGRAM
rm -f $DB

# Create a new database
$PROGRAM $DB c 50 $MAX_ROWS


echo "id age name email"
# simple for loop to fill the new database
for i in `seq 0 $[$MAX_ROWS-1]`;
do
	$PROGRAM $DB s $i $i name$i email$i
done

# list the database entries
output=$($PROGRAM new.dat l)
$PROGRAM $DB l

output=$($PROGRAM $DB g 4)
echo $output
$PROGRAM $DB g 4


echo $($PROGRAM $DB f name name4)
$PROGRAM $DB f name name4

$PROGRAM $DB f email email6

$PROGRAM $DB f age 9
```
This doesn't allow for multiple connections if you wanted to work with a few files at once for example, e.g.: concatenating one part of a file to another.

### Go research "stack data structure" and write one in your favorite language, then try to do it in C.

A good link to the theory of "stack data structure": ```https://en.wikibooks.org/wiki/Data_Structures/Stacks_and_Queues ```.

First implementation in Python: 

```py
#!/usr/bin/env python

class Stack:
    def __init__(self, stack_size):
        self.nodes = []

    def push(self, value):
        self.nodes.append(value)

    def top(self):
        return self.nodes[-1]

    def _pop(self):
        return self.nodes.pop()

    def is_empty(self):
        return self.size() == 0

    def is_full(self):
        return self.size() == stack_size

    def size(self):
        return len(self.nodes)

stack_size = 10
stack = Stack(stack_size)

i = 1
while not stack.is_full():
    stack.push(i)
    i += 1

while not stack.is_empty():
    print stack.top()
    stack._pop()

```

Next, implemented in c using a struct to represent the Stack.

```c

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>


#define STACK_SIZE 10

struct Stack {
	// total size of stack
	int nodes[STACK_SIZE];
	// current size of stack in-use
	int size;
};

struct Stack *stack_init() 
{
	struct Stack *mystack = malloc(sizeof(struct Stack));
	
	assert(mystack != NULL);

	mystack->size = 0;

	return mystack;
}

void close_stack(struct Stack *stack)
{
	if(stack) free(stack);
}

void die(const char *message, struct Stack *stack)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}
	
	close_stack(stack);

	exit(1);
}

void push(struct Stack *stack, int value)
{
	// implements a "is full" function check.
	// hence, no need for separate function.
	// see "stack data structure" for more...
	if(stack->size < STACK_SIZE) {
		stack->nodes[stack->size] = value;
		stack->size++;
	} else {
		die("Stack is full", stack);
	}
}

void pop(struct Stack *stack) 
{
	// implements an "is empty" function check
	// hence, no need for separate function.
	// see "stack data structure" for more...
	if(stack->size > 0) {
		stack->size--;
	} else {
		die("Stack is empty", stack);
	}
}

int top(struct Stack *stack)
{
	return stack->nodes[stack->size-1];
}


int main(int argc, char *argv[])
{
	struct Stack *mystack = stack_init();

	int i = 0;

	puts("PUSH");

	// load up the stack
	for(i = 0; i < STACK_SIZE; i++)
	{	
		// push(struct Stack, int value);
		push(mystack, i);
		printf("Top of stack: %d\n\n", top(mystack));
	}

	puts("POP");

	for(i = 0; i < STACK_SIZE; i++)
	{	
		printf("Top of stack %d\n\n", top(mystack));
		pop(mystack);
	}


	close_stack(mystack);
	return 0;
}

```


