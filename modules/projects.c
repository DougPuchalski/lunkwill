#include "projects.h"

void *projects_close_module(void *arg)
{
	//~ dbgprintf("Project database closed\n%s","");
    return NULL;
}

int print_projects(void *data, int num, char **arguments, char **colnames)
{
	//~ html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<b>%s</b><br>", arguments[3]);
	printf("%s\n", arguments[3]);
}

int projects_init_module(int id)
{
	printf("PROJECT MODULE INITIALIZED AT: %d\n", id);
	modules[id].id=id;
	modules[id].name="Project";
	modules[id].func=projects_request;
	modules[id].description="Project listing and management";
	struct login_data* md=modules[id].data=malloc(sizeof(struct projects_data));

	sighndlr_add(projects_close_module, md);
	return 0;
}

int projects_request(void *module_data, request *client_request)
{
	void *html;

	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<h3 style='margin-left:10px'>","Projects","</h3>");
	html=html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<table style='margin-left:10px'", NULL,"</table>");
	
	//Project list
	const char* data = "Callback function called";
	char *zErrMsg = NULL;
	
	int db_handle, db_status;
	
	/** \todo Read database file from configuration */
	if(sqlite3_open("lunkwill.db", &db_handle))
	{
		fprintf(stderr, "Could not open DB\n");
		return 1;
	}

	db_status = sqlite3_exec(&db_handle, "SELECT * FROM tickets WHERE parent = 0", print_projects, (void*)data, &zErrMsg);
	if(db_status != SQLITE_OK)
	{
		fprintf(stderr, "[FUUU]:\tdb_status = %d\n", db_status);
		
		if(zErrMsg == NULL)
			fprintf(stderr, "[FUCK]:\t");
		
		fprintf(stderr, "sqlite error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(db_handle);

	return 0;

	goto ERROR_SERVER;
	ERROR_SERVER:
		return 2;
}
