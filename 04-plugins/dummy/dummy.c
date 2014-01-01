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

	html_add_tag(&((struct html_ui*)client_request->answer)->main, "<h1>DUMMY MODULE</h1>", NULL, NULL);
	html_add_tag(&((struct html_ui*)client_request->answer)->sidebar, "<h3>Dummy Index</h3>- Option 1<br>- Option 2<br>- Option 3<br>- Option 4<br>- Option 5<br>", NULL, NULL);
	html_add_tag(&((struct html_ui*)client_request->answer)->main, "Select your color!<span style='margin-left: 50px;'> </span>", "<input type='color' id='selected_color' onchange='selectColor()'>", NULL);


	return 0;
}
