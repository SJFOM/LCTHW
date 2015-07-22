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
