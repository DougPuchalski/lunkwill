#include "projects.h"

void *projects_close_module(void *arg)
{
	nfree(arg);
	return NULL;
}

int print_projects(void *data, int num, char **arguments, char **colnames)
{
	//~ html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<b>%s</b><br>", arguments[3]);
	log_write(arguments[3], LOG_DBG, 0);
	return 0;
}

int init_module(int id, struct module_info *m)
{
	char buf[64];
	sprintf(buf, "Project module intialized at %d", id);
	log_write(buf, LOG_INFO, 0);

	m->id=id;
	m->name="Project";
	m->func=projects_request;
	m->description="Project listing and management";
	struct login_data* md=m->data=malloc(sizeof(struct projects_data));

	sighndlr_add(projects_close_module, md);
	return 0;
}

int projects_request(void *md, request *client_request)
{

	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<h3 style='margin-left:10px'>","Projects","</h3>");
	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<table style='margin-left:10px'", NULL,"</table>");

	return 0;
}
