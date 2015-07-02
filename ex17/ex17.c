#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// A reliable way to create constants.

#define MAX_DATA 512
#define MAX_ROWS 100


struct Address {
	int id;
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

void die(const char *message)
{

	/* Usually when you have en error return from a function it will produce an
	error number 'errno' which details the problem. The function 'perror' prints
	the actual error details. The else portion just prints the user-defined error
	message passed to the 'die' function if no explicit errno exists.*/
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}


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
	from the stream pointed to by 'stream' and eventually storing them at
	the location pointed to by 'ptr'. 

	RETURN VALUE: The number of items read.

	In this case, 1 element 'sizeof(struct Database)' bytes long is read
	from the file associated with the current Connection struct and is 
	then stored in the database associated with the same Connection struct.

	rc = 1 is the expected case as you load the entire (single, 1) database 
	everytime you call upon Database_load. Note: Database_get handles this
	differently. */

	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Failed to load database.");
}


/* The 'const' keyword is used here for optimisation purposes. You know that
the name of the file (filename) isn't going to change (or at least you never
want it to change) while you're working on it in this program so it is safe
to assume that it can be constant. 

This allows the compiler to not have to worry about allocating or freeing 
space on the stack (?) and offers some small optimisation. */

struct Connection *Database_open(const char *filename, char mode)
{

	/*  Allocate space for the Connection struct - contains FILE and Database. 
	At this point in the program, Connection only contains two pointers, each
	8 bytes in size - hence, *conn occupies 16 bytes.*/
	struct Connection *conn = malloc(sizeof(struct Connection)); 
	
	/* Catches any malloc errors - could also use: assert(conn != NULL); but
	this simple if statement is already part of the stdio header file. */
	if(!conn) die("Memory error");

	/* conn has had space made for it (16 bytes) but the actual database is
        of much larger size - it's 512 (MAX_DATA) repeated rows of the Address
        struct irrespective of the size of the actual Database - be it only 2 
        or 3 entries. */
        conn->db = malloc(sizeof(struct Database));
        if(!conn->db) die("Memory error");

        printf("sizeof(struct Connection): %ld\n", sizeof(struct Connection));
        printf("sizeof(struct Database): %ld\n", sizeof(struct Database));
	
	printf("Database = (2 ints & 2 char[MAX_ROWS])*MAX_DATA bytes: %d\n", (2*4+2*1*512)*100);


        if(mode == 'c') {
        	conn->file = fopen(filename, "w");
        } else {
        	conn->file = fopen(filename, "r+");

        	if(conn->file) {
        		Database_load(conn);
        	}
        }

        if(!conn->file) die("Failed to open the file");

        return conn;
    }

    void Database_close(struct Connection *conn)
    {
    	if(conn) {
    		if(conn->file) fclose(conn->file);
    		if(conn->db) free(conn->db);
    		free(conn);
    	}
    }

    void Database_write(struct Connection *conn)
    {
    	rewind(conn->file);

    	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    	if(rc != 1) die("Failed to write database.");

    	rc = fflush(conn->file);
    	if(rc == -1) die("Cannot flush database.");
    }

    void Database_create(struct Connection *conn)
    {
    	int i = 0;

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
    	struct Address *addr = &conn->db->rows[id];
    	if(addr->set) die("Already set, delete it first");

    	addr->set = 1;
	// WARNING: bug, read the "How to Break It" and fix this
    	char *res = strncpy(addr->name, name, MAX_DATA);
	// demonstrate the strncpy bug
    	if(!res) die("Name copy failed");

    	res = strncpy(addr->email, email, MAX_DATA);
    	if(!res) die("Email copy failed");
    }

    void Database_get(struct Connection *conn, int id)
    {
    	struct Address *addr = &conn->db->rows[id];

    	if(addr->set) {
    		Address_print(addr);
    	} else {
    		die("ID is not set");
    	}
    }

    void Database_delete(struct Connection *conn, int id)
    {
    	struct Address addr = {.id = id, .set = 0};
    	conn->db->rows[id] = addr;
    }

    void Database_list(struct Connection *conn)
    {
    	int i = 0;
    	struct Database *db = conn->db;

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
    	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

    	char *filename = argv[1];
    	char action = argv[2][0];
    	struct Connection *conn = Database_open(filename, action);
    	int id = 0;

    	if(argc > 3) id = atoi(argv[3]);
    	if(id >= MAX_ROWS) die("There's not that many records");

    	switch(action) {
    		case 'c':
    		Database_create(conn);
    		Database_write(conn);
    		break;
    		
    		case 'g':
    		if(argc != 4) die("Need an id to get");

    		Database_get(conn, id);
    		break;

    		case 's':
    		if(argc != 6) die("Need id, name, email to set");

    		Database_set(conn, id, argv[4], argv[5]);
    		Database_write(conn);
    		break;

    		case 'd':
    		if(argc != 4) die("Need id to delete");

    		Database_delete(conn, id);
    		Database_write(conn);
    		break;

    		case 'l':
    		Database_list(conn);
    		break;
    		default:
    		die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
    		

    	}

    	Database_close(conn);

    	return 0;	
    }
