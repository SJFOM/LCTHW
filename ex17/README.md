#Exercise 17: Heap and Stack Memory Allocation
##How to Break It
The classic way is to remove some of the safety checks such that you can pass in arbitrary data. For example, if you remove the check on line 160 that prevents you from passing in any record number.
i.e. - remove the line:
```c
if(id >= MAX_ROWS) die("There's not that many records.");

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
