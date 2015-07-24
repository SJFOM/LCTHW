#Extra Credit
###Get a hex editor and open up ex18, then find this sequence of hex digits that start a function to see if you can find the function in the raw program.

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
In the compiled binary "ex18" (not ex18.c) you can locate these lines. You must use a hex editor to do so, e.g.: using vim - ":% ! xxd" to enter hex editor mode.

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

###Find other random things in your hex editor and change them. Rerun your program and see what happens. Changing strings you find are the easiest things to change.


