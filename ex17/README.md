#Exercise 17: Heap and Stack Memory Allocation
##How to Break It

###The classic way is to remove some of the safety checks such that you can pass in arbitrary data. For example, if you remove the check on line 160 that prevents you from passing in any record number.
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
###You can also try corrupting the data file. Open it in any editor and change random bytes then close it

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

Addressing the strncpy bug.

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
