#include "modules.h"


int callback(void *a, int b, char **c, char **d)
{
	fprintf(stderr, "login: callback!\n");
	
	return 0;
}


int login_request(request *request)
{
	sqlite3 *sqlite_db;
	char *sqlite_answer;
	char *sqlite_query;
	int commit = sqlite3_open("user.db", &sqlite_db);
	
	if(commit)
	{
		fprintf(stderr, "login: Could not read database\n");
		return 1;
	}
	
	char *a = malloc(BUFSIZ);
	char *errmsg = malloc(BUFSIZ);
	char **err_msg = &errmsg;
	memset(a, 0, BUFSIZ);

	sqlite_query = "SELECT * from user;";
	commit = sqlite3_exec(sqlite_db, sqlite_query, callback, (void*)a, err_msg);
	if(commit != SQLITE_OK ){
		sqlite3_free(err_msg);
		if(commit == 1){
			sqlite_query = "CREATE TABLE IF NOT EXISTS user(ID INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, password TEXT, projects TEXT, admin INTEGER);";
			commit = sqlite3_exec(sqlite_db, sqlite_query, callback, (void*)a, err_msg);			
			if(commit != SQLITE_OK)
			{
				sqlite3_free(err_msg);
			}
			else
			{
				sqlite_query = "INSERT INTO TABLE VALUES(NULL, \"admin\", \"demo\", 1);";
				commit = sqlite3_exec(sqlite_db, sqlite_query, callback, (void*)a, err_msg);
				if(commit != SQLITE_OK)
					sqlite3_free(err_msg);
			}
		}
	}
	

	sqlite3_close(sqlite_db);

	
	return 0;
}
