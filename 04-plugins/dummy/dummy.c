#include "dummy.h"

void *close_module(void *arg)
{
	nfree(arg);
    return NULL;
}

int init_module(int id, struct module_info *m)
{
	printf("DUMMY MODULE INITIALIZED AT: %d\n", id);
	m->id=id;
	m->name="DUMMY";
	m->func=answer_request;
	m->description="Just an example.";
	struct login_data* md=m->data=malloc(sizeof(struct module_data));

	sighndlr_add(close_module, md);
	return 0;
}

int answer_request(void *module_data, request *client_request)
{

	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<h3 style='margin-left:10px'>","DUMMY","</h3>");
	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<table style='margin-left:10px'", NULL,"</table>");
		
	return 0;

	goto ERROR_SERVER;
	ERROR_SERVER:
		return 2;
}
