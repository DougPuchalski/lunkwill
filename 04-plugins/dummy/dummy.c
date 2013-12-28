#include "dummy.h"

void *close_module(void *arg)
{
	nfree(arg);
	return NULL;
}

int init_module(int id, struct module_info *m)
{
	char buf[64];
	sprintf(buf, "Dummy module intialized at %d", id);
	log_write(buf, LOG_INFO, 0);
	m->id=id;
	m->name="DUMMY";
	m->func=answer_request;
	m->description="Just an example.";
	struct module_data* md=m->data=malloc(sizeof(struct module_data));

	sighndlr_add(close_module, md);
	return 0;
}

int answer_request(void *md, request *client_request)
{

	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<h3 style='margin-left:10px'>","DUMMY","</h3>");
	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<table style='margin-left:10px'", NULL,"</table>");

	return 0;
}
