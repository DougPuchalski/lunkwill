/** \file module.c
 *  \brief A sample dummy module
 */

#include "../modules.h"

void *projects_close_module(void *arg);
int projects_get_request(char *request);

/** \brief Initializes the module */
char projects_init_module(int id){
	printf("PROJECT MODULE INITIALIEZED AT: %d\n",id);
	session.module[id]=(void *)projects_get_request;
	sighndlr_add(projects_close_module, NULL);
	
	modules[id].id=id;
	modules[id].name="Project";
	modules[id].description="Manages Projects";
	
	return 1;
}

static int callback(void *data, int argc, char **argv, char **azColName){
	int i;
	if(((char *)data)[0]==0){
		for(i=0; i<argc; i++){
			if(((char *)data)[0]==0){
				strncpy(data, azColName[i],BUF_SIZE);
			}
			else
			{
				strncat(data, azColName[i],BUF_SIZE);
			}
			strncat(data, "\t",BUF_SIZE);
		}
	}

	strncat(data, "\n",BUF_SIZE);

	for(i=0; i<argc; i++){
		strncat(data, argv[i] ? argv[i] : "NULL",BUF_SIZE);
		strncat(data, "\t",BUF_SIZE);
	}
	strncat(data, "\n",BUF_SIZE);
	return 0;
}


/** \brief Parses get requests */
int projects_get_request(char *request){
	char answer[BUF_SIZE],*a,*sql;
	char *http_answer = malloc(BUF_SIZE);	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
   
   
	if (config_lookup_string(&session.config, "DATABASE_TYPE", (const char **)&a))
	{
		if(strcmp(a, "sql3")){
			if (!config_lookup_string(&session.config, "DATABASE", (const char **)&a))
			{
				strcpy(answer,"db.sql3");
				a=answer;
			}
		}
	}
	else
	{
		if (!config_lookup_string(&session.config, "DATABASE", (const char **)&a))
		{
			strcpy(answer,"db.sql3");
			a=answer;
		}
	}	
	
	rc = sqlite3_open(a, &db);
	if(rc)
	{
		nfree(http_answer);
		return 1;
	}
	
	
	a=malloc(BUF_SIZE);
	a[0]=0;

	sql="SELECT * from PROJECTS";
	rc = sqlite3_exec(db, sql, callback, (void*)a, &zErrMsg);
	if( rc != SQLITE_OK ){
		sqlite3_free(zErrMsg);
		if(rc==1){
			sql = "CREATE TABLE PROJECTS("  \
         "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
         "NAME           TEXT    NOT NULL," \
         "DESCRIPTION    TEXT    NOT NULL);";
			rc = sqlite3_exec(db, sql, callback, (void*)a, &zErrMsg);			
			if(rc != SQLITE_OK)
			{
				sqlite3_free(zErrMsg);
			}
			else
			{
				sql = \
				"INSERT INTO PROJECTS (NAME,DESCRIPTION) "  \
				"VALUES ('lunkwill', 'Bugtracking and Projectmanagement' ); ";
				rc = sqlite3_exec(db, sql, callback, (void*)a, &zErrMsg);			
				if(rc != SQLITE_OK) sqlite3_free(zErrMsg);
			}
		}
	}

	sqlite3_close(db);
	send_string(&http_answer, a);
	nfree(a);
	return 1;	
}


/** \brief Exits dummy module */
void *projects_close_module(void *arg){
	return NULL;
}
