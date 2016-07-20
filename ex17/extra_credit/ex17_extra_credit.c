#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
	int id;
	int set;
	int age; // new entry
	char *name;
	char *email;
};

struct Database {
	int MAX_DATA;
	int MAX_ROWS;
	// rows is an array so need two pointers
	struct Address **rows;
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
	printf("%d %d %s %s\n", 
		addr->id, addr->age, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	int rc;
	size_t i = 0;

	/* The Database struct layout - helps determine what needs to be 
	malloc'd.
	struct Database {
		int MAX_DATA;
		int MAX_ROWS;
		struct Address *rows;
	}; */


	// Make sure you've a valid connection.
	if(!(conn->db && conn->file)) die("Database load: Invalid connection", conn);	

	// Stores info from file, MAX_DATA in size to &conn->db->MAX_DATA.
	rc = fread(&conn->db->MAX_DATA, sizeof(conn->db->MAX_DATA), 1, conn->file);
	if(rc!=1) die("Database load: Failed to load MAX_DATA from file.", conn);

	// Stores info from file, MAX_ROWS in size to &conn->db->MAX_ROWS.
	rc = fread(&conn->db->MAX_ROWS, sizeof(conn->db->MAX_ROWS), 1, conn->file);
	if(rc!=1) die("Database load: Failed to load MAX_ROWS from file.", conn);

	// Have set the values of MAX_ROWS and MAX_DATA in the Database struct.
	// Now need to set the size of the rows Address struct so it's usable. 	
	conn->db->rows = (struct Address**) malloc(sizeof(struct Address*) * conn->db->MAX_ROWS);

	if(!conn->db->rows) die("Database load: could not malloc MAX_ROWS Address structs.", conn);

	// Now need to load the data from the file to the rows Address struct.

	for(i = 0; i < conn->db->MAX_ROWS; i++)
	{
		// Need to copy every entry from the file into the Address struct
		conn->db->rows[i] = (struct Address*)malloc(sizeof(struct Address));
		// make things a bit easier
		struct Address *row = conn->db->rows[i];

		// make space for set, id and age.
		rc = fread(&row->id, sizeof(row->id), 1,conn->file);
		if(rc!=1) die("Database load: Failed to load id.", conn);
	
		rc = fread(&row->set, sizeof(row->set), 1, conn->file);
		if(rc!=1) die("Database load: Failed to load set state.", conn);
	
		rc = fread(&row->age, sizeof(row->age), 1, conn->file);
		if(rc!=1) die("Database load: Failed to load age.", conn);

		// need to make space for name and email.

		row->name = malloc(sizeof(char) * conn->db->MAX_DATA);			
		row->email = malloc(sizeof(char) * conn->db->MAX_DATA);		
		if(!(row->name && row->email)){
		die("Database load: Failed to allocate memory for name and email strings in Address.", conn);}

		// read in both strings
		rc = fread(row->name, (sizeof(char/**row->name*/) * conn->db->MAX_DATA), 1, conn->file);
		if(rc!=1) die("Database load: Failed to load name.", conn);

		rc = fread(row->email, (sizeof(char/**row->email*/) * conn->db->MAX_DATA), 1, conn->file);	
		if(rc!=1) die("Database load: Failed to load email.", conn);

		// Need to free each malloc. Could do it here but more proper to do it in Database_close.
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
		if(conn->db && conn->db->rows) {
			size_t i;
			for(i = 0; i < conn->db->MAX_ROWS; i++) {
				
				struct Address *cur = conn->db->rows[i];
				free(cur->name);
				free(cur->email);
				free(cur);}
		}
		if(conn->db->rows) free(conn->db->rows);		
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	} 
}


void Database_write(struct Connection *conn)
{   
	rewind(conn->file);

	size_t i;
	int rc;

	// write MAX_DATA
	rc = fwrite(&conn->db->MAX_DATA, sizeof(conn->db->MAX_DATA), 1, conn->file);
	if(rc != 1) die("Database write: Failed to write MAX_DATA to file.", conn);

	// write MAX_ROWS
	rc = fwrite(&conn->db->MAX_ROWS, sizeof(conn->db->MAX_ROWS), 1, conn->file);
	if(rc != 1) die("Database write: Failed to write MAX_ROWS to file.", conn);
	
	// write Address struct
	for(i = 0; i < conn->db->MAX_ROWS; i++) {

		struct Address *row = conn->db->rows[i];
		// load id
		if(fwrite(&row->id, sizeof(row->id),1,conn->file) != 1)
		die("Database write: could not write id.", conn);

		//load set
		if(fwrite(&row->set, sizeof(row->set),1,conn->file) != 1)
		die("Database write: could not write set.", conn);
	
		//load age
		if(fwrite(&row->age, sizeof(row->age),1,conn->file) != 1)
		die("Database write: could not write age.", conn);

		// load name
		if(fwrite(row->name, sizeof(char)*conn->db->MAX_DATA,1,conn->file) != 1)
		die("Database write: could not write name.", conn);

		// load email
		if(fwrite(row->email, sizeof(char)*conn->db->MAX_DATA,1,conn->file) != 1)
		die("Database write: could not write email.", conn);
	}


	rc = fflush(conn->file);
	if(rc == -1) die("Database write: Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int MAX_DATA, int MAX_ROWS)
{
	// mode == 'c'
	int i = 0;
	
	// in the previous exercises, name and email were of fixed size.
	// here, need to initialise them to their correct sizes and
	// make them blank for now.

	conn->db->MAX_DATA = MAX_DATA;
	conn->db->MAX_ROWS = MAX_ROWS;

	// malloc space for the Address in Database

	conn->db->rows = (struct Address **)malloc(sizeof(struct Address*) * MAX_ROWS);
	
	for(i = 0; i < MAX_ROWS; i++) {

		conn->db->rows[i] = (struct Address*)malloc(sizeof(struct Address));
		
		conn->db->rows[i]->id = i;
		conn->db->rows[i]->set = 0;	
		conn->db->rows[i]->age = -1;
		conn->db->rows[i]->name = (char*)strndup(" ", MAX_DATA);
		conn->db->rows[i]->email = (char*)strndup(" ", MAX_DATA);
	}
}


void Database_set(struct Connection *conn, int id, int age, const char *name, const char *email) 
{  
	// mode == 's'

	if(!(conn)) die("Database set: Could not load conn.", conn);
	if(!(conn->db)) die("Database set: Could not load conn->db.", conn);
	if(!(conn->db->rows))
		 die("Database set: Could not load conn->db->rows.", conn);
	if(!(conn->db->rows[id])) 
		die("Database set: Could not load conn->db->rows[id].", conn);

	struct Address *addr = conn->db->rows[id];
	if(addr->set) die("Database set: Already set, delete it first", conn);

	int MAX_DATA = conn->db->MAX_DATA;

	addr->set = 1;
	addr->age = age;
	addr->name = malloc(sizeof(char) * MAX_DATA);
	addr->email = malloc(sizeof(char) * MAX_DATA);


	char *res = strncpy(addr->name, name, MAX_DATA);
	//addr->name[MAX_DATA-1] = '\0';
	if(!res) die("Database set: Name copy failed", conn);

	res = strncpy(addr->email, email, MAX_DATA);
	//addr->email[MAX_DATA-1] = '\0';
	if(!res) die("Database set: Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
	// mode =='g'
	struct Address *addr = conn->db->rows[id];
	if(addr->set) {
		Address_print(addr);
	} else {
		die("Database get: ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id)
{ 	
	// mode == 'd'	
	conn->db->rows[id]->id = id;
	conn->db->rows[id]->set = 0;
	conn->db->rows[id]->age = 0;
}

void Database_list(struct Connection *conn)
{
	// mode == 'l'
	int i = 0;
	struct Database *db = conn->db;
	int MAX_ROWS = conn->db->MAX_ROWS;
	for (i = 0; i < MAX_ROWS; i++)
	{
		struct Address *cur = db->rows[i];

		if(cur->set) {
			Address_print(cur);
		}
	}
}


void find(int state, const char *search, struct Connection *conn)
{
	int i = 0;
	int found = 0;
	
	struct Database *db = conn->db;

	for(i = 0; i < db->MAX_ROWS; i++)
	{
		struct Address *cur = db->rows[i];
		
		if((!(strcmp(search,cur->name)) && state) 
		|| (!(strcmp(search, cur->email)) && !state) 
		|| (atoi(search) == cur->age && state==2)){
			found = 1;
			Address_print(cur);}
	}
	if(!found) puts("entry not found");
}


int id_error_catch(int id, struct Connection *conn)
{
	if(id < 0 || id >= conn->db->MAX_ROWS) {
		printf("id addressed: %d\nMAX_ROWS: 0 ->  %d\n",
			id, conn->db->MAX_ROWS-1);			
		die("main: There's not that many records", conn); 
		return 0;
	} else { return id; }
	
}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	switch(action) {
		case 'c':
		if(argc < 5) {
			fclose(conn->file);
			free(conn->db);
			free(conn);
			die("USAGE: ex17 <dbfile> c MAX_DATA MAX_ROWS", NULL);
		}

		Database_create(conn, atoi(argv[3]), atoi(argv[4]));
		Database_write(conn);
		break;

		case 'g':
		if(argc != 4) die("Need an id to get", conn);

		id = id_error_catch(atoi(argv[3]), conn);
		Database_get(conn, id);
		break;

		case 's':
		if(argc != 7) die("Need id, age, name, email to set", conn);

		id = id_error_catch(atoi(argv[3]), conn);
		Database_set(conn, id, atoi(argv[4]), argv[5], argv[6]);
		Database_write(conn);
		break;

		case 'd':
		if(argc != 4) die("Need id to delete", conn);
	
		id = id_error_catch(atoi(argv[3]), conn);
		Database_delete(conn, id);
		Database_write(conn);
		break;

		case 'l':
		Database_list(conn);
		break;

		case 'f':
		if(argc < 5) die("USAGE: ex17 <dbfile> f <name/email> \"search term\"", conn);

		const char *term = argv[3];
		const char *search = argv[4];
		if(strcmp(term, "name") == 0) {
			// search for name
			find(1, search, conn);
		} else if(strcmp(term, "email") == 0) {
			// search for email
			find(0, search, conn);
		} else if(strcmp(term, "age") == 0) {
			//search for age
			find(2, search, conn);
		}
		break;

		default:
		die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, f=find", conn);

	}

	Database_close(conn);

	return 0;	
}
