#include "projects.h"

void *projects_close_module(void *arg)
{
	//~ dbgprintf("Project database closed\n%s","");
    return NULL;
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

	return 0;

	goto ERROR_SERVER;
	ERROR_SERVER:
		return 2;
}
