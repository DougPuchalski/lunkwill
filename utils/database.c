#include "database.h"

int silly_sqlite_callback(void *notused, int notusedeither, char **notusedeither2, char **notusedeither3)
{
   return 0;
}


int init_db(void)
{
	sqlite3 *db_handle;
	
	/** \todo Read database file from configuration */
	if(sqlite3_open("lunkwill.db", &db_handle))
	{
		return 0;
	}
	
	sighndlr_add(close_db, db_handle);
	
	char *sql_create = 	"CREATE TABLE IF NOT EXISTS users("\
						"ID INT PRIMARY KEY NOT NULL,"\
						"name TEXT NOT NULL,"\
						"email TEXT NOT NULL,"\
						"password TEXT NOT NULL,"\
						"lastlogin INT NOT NULL,"\
						"currentsession TEXT NOT NULL);"\
						
						"CREATE TABLE IF NOT EXISTS permissions("\
						"ID INT PRIMARY KEY NOT NULL,"\
						"user INT NOT NULL,"\
						"project INT NOT NULL,"\
						"permission INT NOT NULL);"\
						
						"CREATE TABLE IF NOT EXISTS tickets("\
						"ID INT PRIMARY KEY NOT NULL,"\
						"creator TEXT NOT NULL,"\
						"creationtime INT NOT NULL,"\
						"title TEXT NOT NULL,"\
						"text TEXT NOT NULL,"\
						"parent INT NOT NULL,"\
						"assigneduser INT NOT NULL,"\
						"label INT NOT NULL,"\
						"status INT NOT NULL,"\
						"priority INT NOT NULL);";

	char *sql_error = NULL;
	
	if(sqlite3_exec(db_handle, sql_create, silly_sqlite_callback, 0, &sql_error) != SQLITE_OK)
	{
		fprintf(stderr, "sqlite error: %s\n", sql_error);
		sqlite3_free(sql_error);
		
		return 0;
	}

	return 1;
}


void *close_db(void *db_handle)
{
	sqlite3_close(db_handle);
	return NULL;
}
