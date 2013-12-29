#include "login.h"

void *login_close_module(void *arg)
{
	nfree(arg);
	return NULL;
}

int login_init_module(int id)
{
	Modules[id].id=id;
	Modules[id].name="Account";
	Modules[id].func=login_request;
	Modules[id].description="Account settings";
	struct login_data* md=Modules[id].data=malloc(sizeof(struct login_data));

	//init db here
	md->site="<h3>Account</h3>";

	sighndlr_add(login_close_module, md);

	return 0;
}

int login_verify(int uid, int gid, int ses1, int ses2)
{
	return 0;
}

int login_new_session(char *input, int uid, int gid, int ses1, int ses2)
{
	//ask db here
	if(uid==gid&&ses1==ses2)
	{
		strcpy(input, "BAAAAAAAAAAAAAAAAAAA");
		return 0;
	}
	return 1;
}

int user_profile(void *module_data, request *client_request)
{
	html_add_tag(&((struct html_ui*)client_request->answer)->main,NULL,((struct login_data*)module_data)->site,NULL);
	return 0;
}

int login_request(void *module_data, request *client_request)
{
	struct html_ui *html=client_request->answer;
	void *x;
	int retcode=0;

	if(client_request->user==0)
	{
		if(client_request->group==0 && client_request->session1==0 && \
		        client_request->session2==0 && client_request->module_request[0]!=0)
		{
			if((login_new_session(client_request->module_request, client_request->user, \
			                      client_request->group, client_request->session1, \
			                      client_request->session2))!=0)goto ERROR_SERVER;

			html_add_tag(&html->main, "<script>"\
			             "window.setCookie('login','",
			             client_request->module_request,
			             "',7);window.setCookie('module','BA',7);"\
			             "</script>");
		}

		html_add_tag(&html->main, "<script>", "lw_login_form();","</script>");
		return 0;
	}

	if(login_verify(client_request->user, \
	                client_request->group, client_request->session1, \
	                client_request->session2)!=0) goto ERROR_SERVER;


	//MODULE LIST
	int i;
	void *div=html_add_tag(&html->header, \
	                       "<script>window.lw_ModuleList = {",NULL,
	                       "}; window.lw_show_ModuleList();</script>");

	for(i=0; i<256; i++)
	{
		if(Modules[i].name!=NULL)
		{
			x=split_to_xstring(i,URL_CHARS,6,2);
			html_add_tag(&div, "'", x, "':");

			nfree(x);

			html_add_tag(&div, "'",Modules[i].name, "', ");
		}
	}

	//CALL MODULE
	if(Modules[client_request->module].func!=NULL)
	{
		if(client_request->module==0)
		{
			retcode=user_profile(module_data, client_request);
		}
		else
		{
			retcode=Modules[client_request->module].func(\
			        Modules[client_request->module].data, \
			        client_request);
		}
	}

	//LOGOUT BUTTON
	if(retcode==0)
	{
		html_add_tag(&html->header, \
		             "<script>","window.lw_logout_button();","</script>");
	}
	return retcode;

ERROR_SERVER:
	x=html_flush(&html->base,1);
	nfree(x);
	if(!retcode)html_add_tag(&html->base, HTTP_451, "", "");
	return 2;
}
