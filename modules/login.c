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

int login_verify(int uid, int gid, int ses1, int ses2 )
{
	//ask db here
	if(uid!=gid&&ses1==ses2)
	{
		return 0;
	}
	return 1;
}

int login_new_session(char *input, int uid, int gid, int ses1, int ses2 )
{
	//ask db here
	if(uid==gid&&ses1==ses2)
	{
		strcpy(input, "BAAAAAAAAAAAAAAAAAAA");
		return 0;
	}
	return 1;
}

int login_request(void *module_data, request *client_request)
{
	struct login_data *l=module_data;
	
	if(client_request->user==0)
	{
		if(client_request->group==0 && client_request->session1==0 && \
			client_request->session2==0 && client_request->module_request[0]!=0)
		{
			dbgprintf("[LOGIN] %s\n", client_request->module_request);
			if((login_new_session((char*)client_request->module_request, client_request->user, \
				client_request->group, client_request->session1, \
				client_request->session2))!=0)goto ERROR_SERVER;
			return 3;
		}
		return 1;
	}
	return login_verify(client_request->user, \
		client_request->group, client_request->session1, \
		client_request->session2);
	
	ERROR_SERVER:
		return 2;
}
