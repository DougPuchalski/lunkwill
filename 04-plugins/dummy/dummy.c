#include "dummy.h"

void *close_module(void *arg)
{
	nfree(((struct module_info*)arg)->data);
	nfree(arg);
	return NULL;
}

int init_module(int id, struct module_info *m)
{
	char buf[64];
	sprintf(buf, "Dummy module intialized at %d", id);
	log_write(buf, LOG_INFO);
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
	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<b>Dummy Index</b>", NULL, NULL);
	html_add_tag(&((struct html_ui*)client_request->answer)->main, "<table style='margin-left:10px'", NULL,"</table>");
	html_add_tag(&((struct html_ui*)client_request->answer)->main, "Select your color!<span style='margin-left: 50px;'> </span>", "<input type='color' id='selected_color' onchange='selectColor()'>", NULL);


	return 0;
}
