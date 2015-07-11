#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


//#define MAX_DATA 512
//#define MAX_ROWS 100


struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
	int MAX_DATA;
	int MAX_ROWS;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{

	if(errno) {
		perror(message);
	} else {
		printf("die: ERROR: %s\n", message);
	}

	Database_close(conn);

	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", 
		addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	int rc;

	/* The Database struct layout - helps determine what needs to be 
	malloc'd.
	struct Database {
		int MAX_DATA;
		int MAX_ROWS;
		struct Address *rows;
	}; */


	// Make sure you've a valid connection.
	if(!(conn->db && conn->file)) die("Database load: Invalid connection");	

	// Stores info from file, MAX_DATA in size to &conn->db->MAX_DATA.
	rc = fread(&conn->db->MAX_DATA, sizeof(conn->db->MAX_DATA), 1, conn->file);
	if(rc!=1) die("Database load: Failed to load MAX_DATA from file.");

	// Stores info from file, MAX_ROWS in size to &conn->db->MAX_ROWS.
	rc = fread(&conn->db->MAX_ROWS, sizeof(conn->db->MAX_ROWS), 1, conn->file);
	if(rc!=1) die("Database load: Failed to load MAX_ROWS from file.");

	// Have set the values of MAX_ROWS and MAX_DATA in the Database struct.
	// Now need to set the size of the rows Address struct so it's usable. 	
	conn->db->rows = malloc(sizeof(struct Address) * conn->db->MAX_ROWS);

	// Now need to load the data from the file to the rows Address struct.

	for(int i = 0; i < conn->db->MAX_ROWS; i++)
	{
		// Need to copy every entry from the file into the Address struct
		conn->db->rows[i] = malloc(struct Address);
		// make things a bit easier
		struct Address *row = conn->db->rows[i];

		rc = fread(&row->id, sizeof(row->id), 1, db->file);
		if(rc!=1) die("Database load: Failed to load id.");
	
		rc = fread(&row->set, sizeof(row->set), 1, db->file);
		if(rc!=1) die("Database load: Failed to load set state.");

		// need to make space for name and email.

		row->name = malloc(sizeof(*row->name) * conn->db->MAX_DATA);			
		row->email = malloc(sizeof(*row->email) * conn->db->MAX_DATA);		

		if(!(row->name && row->email)){
			 die("Database load: Failed to allocate memory for name and email strings in Address.");}

		// read in both strings
		rc = fread(row->name, (sizeof(*row->name) * conn->db->MAX_DATA), 1, db->file);
		if(rc!=1) die("Database load: Failed to load name.");

		rc = fread(row->email, (sizeof(*row->email) * conn->db->MAX_DATA), 1, db->file);	
		if(rc!=1) die("Database load: Failed to load email.");

	}
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection)); 
	
	if(!conn) die("Database open: Memory error", conn);

	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) die("Database open: Memory error", conn);	

	if(mode == 'c') {

		conn->file = fopen(filename, "w");
	} else {

		conn->file = fopen(filename, "r+");

		if(conn->file) {
			Database_load(conn);
		}
	}

	if(!conn->file) die("Database open: Failed to open the file", conn);

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
	if(rc != 1) die("Database write: Failed to write database.", conn);
	
	rc = fflush(conn->file);
	if(rc == -1) die("Database write: Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
	int i = 0;
	
	for(i = 0; i < MAX_ROWS; i++) {
		struct Address addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}


void Database_set(struct Connection *conn, int id, const char *name,
	const char *email) 
{  
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Database set: Already set, delete it first", conn);

	addr->set = 1;

	char *res = strncpy(addr->name, name, MAX_DATA);
	// Apply the fix.
	addr->name[MAX_DATA-1] = '\0';
	// demonstrate the strncpy bug
	if(!res) die("Database set: Name copy failed", conn);

	res = strncpy(addr->email, email, MAX_DATA);
	// Apply the fix.
	addr->email[MAX_DATA-1] = '\0';
	if(!res) die("Database set: Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) {
		Address_print(addr);
	} else {
		die("Database get: ID is not set", conn);
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
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	if(argc > 3) id = atoi(argv[3]);
	//if(id >= MAX_ROWS) die("There's not that many records", conn);

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
