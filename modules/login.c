#include "modules.h"

struct login_data
{
	sqlite3 *sqlite_db;
	pthread_mutex_t db_lock;
};

/** \brief Exits login module */
void *login_close_module(void *arg){
		sqlite3_close(((struct login_data*)arg)->sqlite_db);
		dbgprintf("Closing login db\n%s","");
        return NULL;
}

int callback(void *a_param, int argc, char **argv, char **column){
    int i;
    for ( i=0; i< argc; i++)
        dbgprintf("%s,\t", argv[i]);
    dbgprintf("%s", "\n");
    return 0;
}

int login_init_module(int id)
{
	char a[BUFSIZ];
	char *errmsg;

	printf("LOGIN MODULE INITIALIZED AT: %d\n", id);
	modules[id].id=id;
	modules[id].name="Account";
	modules[id].func=login_request;
	modules[id].description="Account settings";
	struct login_data* md=modules[id].data=malloc(sizeof(struct login_data));

	if(sqlite3_open("lunkwill.db", &md->sqlite_db))
	{
		fprintf(stderr, "login: Could not read database\n");
		return 1;
	}

	pthread_mutex_init ( &md->db_lock, NULL);
	
	sighndlr_add(login_close_module, md);

	char *sqlite_query = "CREATE TABLE IF NOT EXISTS "\
		"users(ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
			"username TEXT, password TEXT, projects TEXT,"\
		" admin INTEGER);";

	if(sqlite3_exec(md->sqlite_db, sqlite_query, callback, (void*)a, &errmsg) != SQLITE_OK)
	{
		sqlite3_free(errmsg);
		return 1;
	}

	return 0;
}


int login_request(void *module_data, request *client_request)
{
	char *sqlite_query=NULL;
	char *errmsg=NULL;
	char a[BUFSIZ];
	struct login_data *l=module_data;
	
	if(client_request->user==0)
	{
		return 1;
	}

	pthread_mutex_lock( &l->db_lock );

		sqlite_query = "SELECT * from users;";
		if(sqlite3_exec(l->sqlite_db, sqlite_query, callback, (void*)a, &errmsg) != SQLITE_OK)
		{
			dbgprintf("[FAIL] SELECT %s\n", errmsg);
			goto ERROR_SERVER;
		}

		sqlite_query = "INSERT INTO users VALUES(NULL, \"admin\", \"demo\", \"\", 1);";
		if(sqlite3_exec(l->sqlite_db, sqlite_query, callback, (void*)a, &errmsg) != SQLITE_OK)
		{
			dbgprintf("[FAIL] INSERT %s\n",errmsg);
			goto ERROR_SERVER;
		}

	pthread_mutex_unlock( &l->db_lock );
	
	return 1;
	
	ERROR_SERVER:
		sqlite3_free(errmsg);
		pthread_mutex_unlock( &l->db_lock );
		return 2;
}
