#include "projects.h"

void *projects_close_module(void *arg)
{
	nfree(arg);
    return NULL;
}

int print_projects(void *data, int num, char **arguments, char **colnames)
{
	//~ html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<b>%s</b><br>", arguments[3]);
	printf("%s\n", arguments[3]);
	return 0;
}

int init_module(int id, struct module_info *m)
{
	printf("PROJECT MODULE INITIALIZED AT: %d\n", id);
	m->id=id;
	m->name="Project";
	m->func=projects_request;
	m->description="Project listing and management";
	struct login_data* md=m->data=malloc(sizeof(struct projects_data));

	sighndlr_add(projects_close_module, md);
	return 0;
}

int projects_request(void *module_data, request *client_request)
{

	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<h3 style='margin-left:10px'>","Projects","</h3>");
	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<table style='margin-left:10px'", NULL,"</table>");
		
	return 0;

	goto ERROR_SERVER;
	ERROR_SERVER:
		return 2;
}
