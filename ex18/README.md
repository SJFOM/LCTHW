# Extra Credit
### Get a hex editor and open up ex18, then find this sequence of hex digits that start a function to see if you can find the function in the raw program.

Running ex18 gives:

```
➜  ex18 git:(master) ✗ ./ex18 1 4 2 9 0
01249
55:48:89:e5:89:7d:fc:89:75:f8:8b:75:fc:2b:75:f8:89:f0:5d:c3:66:66:66:2e:0f:

94210
55:48:89:e5:89:7d:fc:89:75:f8:8b:75:f8:2b:75:fc:89:f0:5d:c3:66:66:66:2e:0f:

49210
55:48:89:e5:89:7d:f8:89:75:f4:81:7d:f8:00:00:00:00:0f:84:0d:00:00:00:81:7d:
```
We can see that each line starts with 55 but have different bodies and endings. The first line here represents:

```c
test_sorting(numbers, count, sorted_order);
```
In the compiled binary "ex18" (not ex18.c) you can locate these lines. You must use a hex editor to do so, e.g.: using vim - ":% ! xxd" to enter hex editor mode (":% ! xxd -r" to exit).

```
"test_sorting(numbers, count, sorted_order);"
0000c10: 5548 89e5 897d fc89 75f8 8b75 fc2b 75f8  UH...}..u..u.+u.
0000c20: 89f0 5dc3 6666 662e 0f1f 8400 0000 0000  ..].fff.........

"test_sorting(numbers, count, reverse_order);"
0000c30: 5548 89e5 897d fc89 75f8 8b75 f82b 75fc  UH...}..u..u.+u.
0000c40: 89f0 5dc3 6666 662e 0f1f 8400 0000 0000  ..].fff.........

"test_sorting(numbers, count, strange_order);"
0000c50: 5548 89e5 897d f889 75f4 817d f800 0000  UH...}..u..}....
0000c60: 000f 840d 0000 0081 7df4 0000 0000 0f85  ........}.......

```

### Find other random things in your hex editor and change them. Rerun your program and see what happens. Changing strings you find are the easiest things to change.

By changing some of the hex strings controlling the "USAGE" warning string won't display properly.

```
➜  ex18 git:(master) ✗ ./ex18
ERROR: USAGE: ex18 4 3 1 5 6
➜  ex18 git:(master) ✗ ./ex18_corrupt 
ERROR: USAGE:

```

This was done by altering the hex code:

```
0000f80: 5553 4147 453a 2065 7831 3820 3420 3320  USAGE: ex18 4 3 
0000f90: 3120 3520 3600 0000 0100 0000 1c00 0000  1 5 6...........
```

to:

```
0000f80: 5553 4147 453a 0000 0000 3820 3420 3320  USAGE:....8 4 3 
0000f90: 0000 0000 0000 0000 0100 0000 1c00 0000  ................
```

### Pass in the wrong function for the compare_cb and see what the C compiler complains about.

Add a "bad callback" function that doesn't match the compare_cb typedef of (int a, int b):

```c
// Extra Credit - pass a bad function to the compare_cb
int bad_callback(int a, int b, int c)
{
	return a + b + c;
}
```

And then in int main added the call: 

```c
// Extra Credit - pass a bad function to the compare_cb
	test_sorting(numbers, count, bad_callback);

```
The C compiler complains in a way you would expect, complaining about incompatible types:

```
➜  extra_credit git:(master) ✗ make
cc -Wall -g    ex18_extra_cred.c   -o ex18_extra_cred
ex18_extra_cred.c:126:31: warning: incompatible pointer types passing
      'int (int, int, int)' to parameter of type 'compare_cb'
      (aka 'int (*)(int, int)') [-Wincompatible-pointer-types]
        test_sorting(numbers, count, bad_callback);
                                     ^~~~~~~~~~~~
ex18_extra_cred.c:81:55: note: passing argument to parameter 'cmp' here
void test_sorting(int *numbers, int count, compare_cb cmp)
                                                      ^
1 warning generated.
```

### Pass in NULL and watch your program seriously bite it. Then run Valgrind and see what that reports.

Added a call in int main to use ```NULL``` for compare_cb; resulting output is a seg fault:

```
➜  extra_credit git:(master) ✗ ./ex18_extra_cred 1 2 3 4
1234
55:48:89:e5:89:7d:fc:89:75:f8:8b:75:fc:2b:75:f8:89:f0:5d:c3:66:66:66:2e:0f:

4321
55:48:89:e5:89:7d:fc:89:75:f8:8b:75:f8:2b:75:fc:89:f0:5d:c3:66:66:66:2e:0f:

4321
55:48:89:e5:89:7d:f8:89:75:f4:81:7d:f8:00:00:00:00:0f:84:0d:00:00:00:81:7d:

[1]    4529 segmentation fault  ./ex18_extra_cred 1 2 3 4
```

Valgrind isn't a fan of that:

```
==4539== Jump to the invalid address stated on the next line
==4539==    at 0x0: ???
==4539==    by 0x100000CA9: test_sorting (ex18_extra_cred.c:84)
==4539==    by 0x100000E95: main (ex18_extra_cred.c:129)
==4539==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==4539== 
==4539== 
==4539== Process terminating with default action of signal 11 (SIGSEGV)
==4539==  Bad permissions for mapped region at address 0x0
==4539==    at 0x0: ???
==4539==    by 0x100000CA9: test_sorting (ex18_extra_cred.c:84)
==4539==    by 0x100000E95: main (ex18_extra_cred.c:129)
==4539== 
==4539== HEAP SUMMARY:
==4539==     in use at exit: 38,481 bytes in 417 blocks
==4539==   total heap usage: 520 allocs, 103 frees, 45,505 bytes allocated
==4539== 
==4539== LEAK SUMMARY:
==4539==    definitely lost: 0 bytes in 0 blocks
==4539==    indirectly lost: 0 bytes in 0 blocks
==4539==      possibly lost: 0 bytes in 0 blocks
==4539==    still reachable: 32 bytes in 2 blocks
==4539==         suppressed: 38,449 bytes in 415 blocks
```

The jump to the invalid address at 0x0 refers to the ```NULL``` pointer address. Bad permissions error is probably to do with 0x0 being a read only space in memory but the function is attempting to alter it which is a definite no-no.

### Write another sorting algorithm, then change test_sorting so that it takes both an arbitrary sort function and the sort function's callback comparison. Use it to test both of your algorithms.

Implemented a "Comb Sort" algorithm, see: ``` https://en.wikipedia.org/wiki/Comb_sort ```. 

A new typedef is required to create a pointer to a function pointer such that ```comb_sort``` is the same type as ```bubble_sort```. The new typedef is:

```c
// Extra Credit - create a pointer to a function pointer for the
// sorting algorithm to use
typedef int *(*sort_cb)(int *numbers, int count, compare_cb cmp);
```

The algorithm implementation is as follows:

```c
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

```

The output of ./ex18_extra_credit is now:

```
➜  extra_credit git:(master) ✗ ./ex18_extra_cred 1 4 2 5 6 8 9

Using Bubble Sort method

Sorted order
1245689
Reverse order
9865421
Strange order
8654291

Using Comb Sort method

Sorted order
1245689
Reverse order
9865421
Strange order: causes infinite loop, don't use with Comb Sort...
```

Currently some issues with Strange order using Comb Sort causing an infinite loop...
