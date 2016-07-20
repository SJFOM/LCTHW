#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** Our old friend die from ex17. */
void die(const char *message)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	exit(1);

}

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);


// Extra Credit - create a pointer to a function pointer for the
// sorting algorithm to use
typedef int *(*sort_cb)(int *numbers, int count, compare_cb cmp);

/**
 * A classic bubblesort function that uses the
 * compare_cb to do the sorting.
 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
	int temp = 0;
	int i = 0;
	int j = 0;
 	int *target = malloc(count * sizeof(int));

	if(!target) die("Memory error.");

	memcpy(target, numbers, count * sizeof(int));

	for(i = 0; i < count; i++) {
		for(j = 0; j < count - 1; j++) {
			if(cmp(target[j], target[j+1]) > 0) {
				temp = target[j+1];
				target[j+1] = target[j];
				target[j] = temp;
			}
		}
	}

	return target;
}

int *comb_sort(int *numbers, int count, compare_cb cmp)
{	
	int temp = 0;
	int i = 0;
	int j = 0;
 	int *target = malloc(count * sizeof(int));
	int swapped;
	double shrink = 1.3;
	int gap = count;

	if(!target) die("Memory error.");

	memcpy(target, numbers, count * sizeof(int));

	for(;;) {
		
		// update the gap value for the next comb
		gap = (int)gap/shrink;

		if(gap < 1) gap = 1; // min gap is 1

		swapped = 0;

		// a single "comb" over the input list
		for(i = 0;(i+gap) < count; i++){
			
			j = i+gap;

			if(cmp(target[i], target[j]) > 0) {
				temp = target[i];
				target[i] = target[j];
				target[j] = temp;							
				// flag that a swap has occurred
				swapped = 1;
			}
		}
		
		if(gap == 1 && !swapped) break;
	}
	return target;
}


int sorted_order(int a, int b)
{
	return a - b;
}

int  reverse_order(int a, int b)
{
	return b - a;
}

int strange_order(int a, int b)
{
	if(a == 0 || b == 0) {
		return 0;
	} else {
		return a % b;
	}
}

/**
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int *numbers, int count, compare_cb cmp, sort_cb scb)
{
	int i = 0;
	//int *sorted = bubble_sort(numbers, count, cmp);
	int *sorted = scb(numbers, count, cmp);

	if(!sorted) die("Failed to sort as requested.");

	for(i = 0; i < count; i++) {
		printf("%d", sorted[i]);
	}	
	printf("\n");

	free(sorted);
	
}


int main(int argc, char *argv[])
{
	if(argc < 2) die("USAGE: ex18 4 3 1 5 6");

	int count = argc - 1;
	int i = 0;
	char **inputs = argv + 1;

	int *numbers = malloc(sizeof(int) * count);
	if(!numbers) die("Memory error.");

	for(i = 0; i < count; i++) {
		numbers[i] = atoi(inputs[i]);
	}

	puts("\nUsing Bubble Sort method\n");

	puts("Sorted order");
	test_sorting(numbers, count, sorted_order, bubble_sort);
	puts("Reverse order");
	test_sorting(numbers, count, reverse_order, bubble_sort);
	puts("Strange order");
	test_sorting(numbers, count, strange_order, bubble_sort);

	puts("\nUsing Comb Sort method\n");

	puts("Sorted order");
	test_sorting(numbers, count, sorted_order, comb_sort);
	puts("Reverse order");
	test_sorting(numbers, count, reverse_order, comb_sort);
	puts("Strange order: causes infinite loop, don't use with Comb Sort...");
	//test_sorting(numbers, count, strange_order, comb_sort);

	free(numbers);	

	return 0;
}
