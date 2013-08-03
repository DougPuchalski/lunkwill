#include "../modules.h"


int login_request(request *request)
{
	sqlite3 *sqlite_db;
	char *sqlite_answer;
	char *sqlite_query;
	int commit = sqlite3_open(sqlite_answer, &sqlite_db);
	
	if(commit)
	{
		fprintf(stderr, "login: Could not read database\n");
		return 1;
	}
	
	a=malloc(BUFSIZ) = {0};

	sql="SELECT * from user;";
	commit = sqlite3_exec(sqite_db, sqlite_query, callback, (void*)a, &zErrMsg);
	if(commit != SQLITE_OK ){
		sqlite3_free(zErrMsg);
		if(commit == 1){
			sql_query = "CREATE TABLE IF NOT EXISTS user(ID INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, password TEXT, projects TEXT, admin INTEGER);";
			commit = sqlite3_exec(sqlite_db, sql_query, callback, (void*)a, &zErrMsg);			
			if(commit != SQLITE_OK)
			{
				sqlite3_free(zErrMsg);
			}
			else
			{
				sql_query = "INSERT INTO TABLE VALUES(NULL, \"admin\", \"demo\", 1);";
				commit = sqlite3_exec(sqlite_db, sql_query, callback, (void*)a, &zErrMsg);			
				if(commit != SQLITE_OK) sqlite3_free(zErrMsg);
			}
		}
	}
	

	sqlite3_close(db);

	
	return 0;
}
