#*Exercise 17: Heap and Stack Memory Allocation*

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

###You could also find ways to pass bad arguments to the program when it's run, such as getting the file and action backwards will make it create a file named after the action, then do an action based on the first character.

```
➜  ex17 git:(master) ✗ ./ex17 c case.dat
➜  ex17 git:(master) ✗ ./ex17 c c
c            corrupt.dat
```

###There is a bug in this program because of strncpy being poorly designed. Go read about strncpy then try to find out what happens when the name or address you give is greater than 512 bytes. Fix this by simply forcing the last character to '\0' so that it's always set no matter what (which is what strncpy should do).

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

###In the extra credit I have you augment the program to create arbitrary size databases. Try to see what the biggest database is before you cause the program to die for lack of memory from malloc.

Keep increasing MAX_DATA and MAX_ROWS until your memory runs out - depends on how much RAM you have!

##Extra Credit

###The die function needs to be augmented to let you pass the conn variable so it can close it and clean up.

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

###Change the code to accept parameters for MAX_DATA and MAX_ROWS, store them in the Database struct, and write that to the file, thus creating a database that can be arbitrarily sized.

```
See ex17_extra_credit.c
```
###Add more operations you can do on the database, like find.

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


###Read about how C does it's struct packing, and then try to see why your file is the size it is. See if you can calculate a new size after adding more fields.

Found a very good article that explains the topic well.

```url
http://www.catb.org/esr/structure-packing/
```
Created a file named struct_padding.c in the extra_credit directory to explore bit padding in structs.

