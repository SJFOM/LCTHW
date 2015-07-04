#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// A reliable way to create constants.

#define MAX_DATA 512
#define MAX_ROWS 100


struct Address {
	int id;
	// set tells the caller whether the id is taken or not (1 or 0).
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database {

	/* A struct of fixed size. Database represents an array of Address structs
	of length MAX_ROWS. This is pretty inefficient if the Database isn't going
	to ever be that big but allows for the entire Database to be created later
	on in Database_write in one simple move - nice! */
	struct Address rows[MAX_ROWS];
};

struct Connection {

	/* FILE is a special type of struct that comes as part of the standard
	C library.  */

	FILE *file;
	struct Database *db;
};

// Create a prototype for Database_close so you can call it in the die function below.
void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{

	/* Usually when you have en error return from a function it will produce an
	error number 'errno' for that  problem. The function 'perror' prints some
	extra info about the error given a valid errno. The else portion just prints
	the user-defined error message passed to the 'die' function if no explicit
	errno exists.

	UPDATE: perror(message) prints "message:details about error" where the details
	are taken from errno. message here could simply be an empty string ("") but
	errno may not always exist so it is good practice to give one anyway.*/
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}


	// Need to call Database_close to clean the program before exiting!
	Database_close(conn);

	/* void _exit(int status)
	The value of 'status' is returned to the parent process as the process' exit
	status - i.e. it's reason for exiting. */
	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", 
		addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	/* size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream); 

	function fread reads 'nmemb' elements of data, each 'size' bytes long,
	from the stream pointed to by 'stream' and eventually stores them at
	the location pointed to by 'ptr'. 

	RETURN VALUE: The number of items read.

	In this case, 1 element 'sizeof(struct Database)' bytes long is read
	from the file associated with the current Connection struct and is 
	then stored in the database associated with the same Connection struct.

	rc = 1 is the expected case as you load the entire (single, 1) database 
	everytime you call upon Database_load. Note: Database_get handles this
	differently. */

	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Failed to load database.", conn);
}



/* The 'const' keyword is used here for optimisation purposes. You know that
the name of the file (filename) isn't going to change (or at least you never
want it to change) while you're working on it in this program so it is safe
to assume that it can be constant. 

This allows the compiler to not have to worry about allocating or freeing 
space on the stack (?) and offers some small optimisation. */

struct Connection *Database_open(const char *filename, char mode)
{

	/*  Allocate space for a pointer to the Connection struct: contains pointers
	to FILE and Database structs. 
	At this point in the program, Connection only contains two pointers, each
	8 bytes in size - hence, *conn occupies 16 bytes.*/
	struct Connection *conn = malloc(sizeof(struct Connection)); 
	
	/* Catches any malloc errors - could also use: assert(conn != NULL); but
	this simple if statement is already part of the stdio header file. */
	if(!conn) die("Memory error", conn);

	/* conn has had space made for it (16 bytes) but the actual database is
        of much larger size - it's 512 (MAX_DATA) repeated rows of the Address
        struct irrespective of the size of the actual Database - be it only 2 
        or 3 entries, hence why this program is not considered efficient. */
        conn->db = malloc(sizeof(struct Database));
        if(!conn->db) die("Memory error", conn);

	/*
	Some test code to prove that Database is of expected byte size.
        printf("sizeof(struct Connection): %ld\n", sizeof(struct Connection));
        printf("sizeof(struct Database): %ld\n", sizeof(struct Database));
	
	printf("Database = (2 ints & 2 char[MAX_ROWS])*MAX_DATA bytes: %d\n", (2*4+2*1*512)*100);
	*/


	// mode 'c' is for creating a new database file (not a new entry - see 's' functionality).
        if(mode == 'c') {
		/* Recall that Database_open is called every time this program is run (in int main).
		Because of this, a special check must be done in the case that a new database is
		being created or whether an already existing one is being used.

		FILE *fopen(const char *path, const char *mode)
		Here, the 'w' mode is set to create a text file of name 'filename' for writing to.
		The text 'stream' is positioned at the start of the file. */
        	conn->file = fopen(filename, "w");
        } else {

		/* If any mode other than 'c' is called by the user, the default is to call fopen
		with full read and write priveledges as per the 'r+' keyword.

		NOTE: if filename does not already exist then the fopen call with the 'r+' keyword
		will return a NULL pointer. This does not mean the program will crash all by itself
		if this happens so a check for the pointer conn->file!=NULL is necessary. */
        	conn->file = fopen(filename, "r+");

		
		/* If a connection to the file does exist, you've worked with it before and so you 
		can load its database contents. */
        	if(conn->file) {
        		Database_load(conn);
        	}
        }
	// Catches a possible stray NULL pointer from the fopen(filename, "r+") call.
        if(!conn->file) die("Failed to open the file", conn);


	/* return the pointer to the Connection struct which has: made space for the conn pointers
	to Database *db and FILE *filename, made space for the Database, ensured a valid filename
	exists for further use in the program - nice! */
        return conn;
    }


    // Need to clean up after all those mallocs
    void Database_close(struct Connection *conn)
    {
	/* conn should always exist but just in case something goes wrong you don't want to be 
	freeing random memory blocks. */	
    	if(conn) {

		/* int fclose(FILE *fp)
		fclose flushes the current text stream pointed to by 'fp' and closes the
		underlying file descriptor - 'filename' in this case. */
    		if(conn->file) fclose(conn->file);
    		if(conn->db) free(conn->db);
		// Free the pointers associated with conn (to db and filename).
    		free(conn);
	} 
    }




    void Database_write(struct Connection *conn)
    {
	/* void rewind(FILE *stream)
	sets the file position indicator for the stream to the start of the file. */    
	rewind(conn->file);

	
	/* size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream); 

	function fwrite writes 'nmemb' elements of data, each 'size' bytes long,
	to the stream pointed to by 'stream' obtaining them from the location 
	pointed to by 'ptr'.  

	RETURN: the number of items written - in this case, 1 database should be 
	written so rc = 1 denotes a successful write. */
    	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    	if(rc != 1) die("Failed to write database.", conn);
	
	/* Anything extraneous to the data already written to conn->file as per
	fwrite above is stored in a buffer.

	Think of fwrite as a note to the machine to "remember to write that
	stream to that file later on once the buffer is full". This is because 
	storing a certain (fixed) amount of bytes to memory first is faster
	than random amounts every time. fflush empties this buffer and causes the
	contents to be written into the file immediately. This is a tradeoff
	between performance and correct program operation, but here in this instance,
	since we don't want anything other than the database information entering the
	file, we use fflush.

	In a nutshell: If you want to force disk storage, as we do here, use fflush.  */
	
    	rc = fflush(conn->file);
	/* -1 indicates an EOF error and is the given return value of fflush upon 
	unsucessful completion. */
    	if(rc == -1) die("Cannot flush database.", conn);
    }

    void Database_create(struct Connection *conn)
    {
    	int i = 0;
	
	// This loop creates a Database with empty id'd  unset (set=0) rows.
    	for(i = 0; i < MAX_ROWS; i++) {
		// make a prototype to initialize it
    		struct Address addr = {.id = i, .set = 0};
		// then just assign it
    		conn->db->rows[i] = addr;
    	}
    }


    void Database_set(struct Connection *conn, int id, const char *name,
    	const char *email) 
    {
	/* addr is set to the address of the database entry at the given
	id address. The database has a fixed number of id entries listed
	from 0-99. */    
	struct Address *addr = &conn->db->rows[id];
	/* The 'set' keyword is toggled between 1 and 0 where 1 means an
	entry has been already given an id, name and email. If this is 
	the case and an overwrite is attempted, the if statement is true
	and the user is prompted to first remove that entry.*/
    	if(addr->set) die("Already set, delete it first", conn);


	/* If the code passes the previous if statement then it is assumed
	that the requested id number was not taken and can now be toggled 
	as set (as 1).*/
    	addr->set = 1;

	// WARNING: bug, read the "How to Break It" and fix this
	/* if the length of name exceeds MAX_DATA, then no terminating NULL
	character is found ('\0') and the resulting addr->name won't be 
	NULL terminated either.

	You can fix this by setting the last entry in name as a NULL character
	so this never happens. */

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

    void Database_get(struct Connection *conn, int id)
    {
    	struct Address *addr = &conn->db->rows[id];
	// check whether the id exists - can't print it if it's not there!
    	if(addr->set) {
    		Address_print(addr);
    	} else {
    		die("ID is not set", conn);
    	}
    }

    void Database_delete(struct Connection *conn, int id)
    {
	// Set that id entry to zero - un-initialize it.    
	struct Address addr = {.id = id, .set = 0};
    	conn->db->rows[id] = addr;
    }

    void Database_list(struct Connection *conn)
    {
    	int i = 0;
    	struct Database *db = conn->db;
	
	// Runs through the database and prints any existing entries (where set=1).
    	for (i = 0; i < MAX_ROWS; i++)
    	{
    		struct Address *cur = &db->rows[i];

    		if(cur->set) {
    			Address_print(cur);
    		}
    	}
    }


    int main(int argc, char *argv[])
    {
	// must use null as second parameter as no Connection struct has yet been made.
    	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    	char *filename = argv[1];
	// the first character ([0]) in the second string ([2]).
    	char action = argv[2][0];
    	struct Connection *conn = Database_open(filename, action);
    	int id = 0;

    	if(argc > 3) id = atoi(argv[3]);
    	if(id >= MAX_ROWS) die("There's not that many records", conn);

    	switch(action) {
    		case 'c':
    		Database_create(conn);
    		Database_write(conn);
    		break;
    		
    		case 'g':
    		if(argc != 4) die("Need an id to get", conn);

    		Database_get(conn, id);
    		break;

    		case 's':
    		if(argc != 6) die("Need id, name, email to set", conn);

    		Database_set(conn, id, argv[4], argv[5]);
    		Database_write(conn);
    		break;

    		case 'd':
    		if(argc != 4) die("Need id to delete", conn);

    		Database_delete(conn, id);
    		Database_write(conn);
    		break;

    		case 'l':
    		Database_list(conn);
    		break;
    		default:
    		die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    		

    	}

    	Database_close(conn);

    	return 0;	
    }
