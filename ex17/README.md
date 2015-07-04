#Exercise 17: Heap and Stack Memory Allocation
##How to Break It
The classic way is to remove some of the safety checks such that you can pass in arbitrary data. For example, if you remove the check on line 160 that prevents you from passing in any record number.
i.e. - remove the line:
```
if(id >= MAX_ROWS) die("There's not that many records.");

```
