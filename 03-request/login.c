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

int login_verify(int uid, int gid, int ses1, int ses2 )
{
	return 0;
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
				"createCookie('login','",
				client_request->module_request,
				"',7);createCookie('module','BA',7);"\
				"</script>");
		}

		void *form=html_add_tag(&html->main, \
		"<form action='javascript:get_url("\
		"(document.getElementById(\"email\").value+document.getElementById(\"password\").value)"\
		"),get_url(\"\")'>","","</form>");
		html_add_tag(&form, "<strong>E-Mail:</strong><br><input id=email type=email>","","</input><p>");
		html_add_tag(&form, "<strong>Password:</strong><br><input id=password type=password>","","</input><p>");
		html_add_tag(&form, "<input type=submit value=Login>",NULL,"</input>");
		return 0;
	}

	if(login_verify(client_request->user, \
		client_request->group, client_request->session1, \
		client_request->session2)!=0) goto ERROR_SERVER;


	//MODULE LIST
	int i;
	void *div=html_add_tag(&html->header, \
		"<div id='ModuleList'"\
		"style='display:inline-block;overflow-x:auto;margin-left:5px;width:90%'>", \
		NULL,"</div>");

	for(i=0;i<256;i++)
	{
		if(Modules[i].name!=NULL)
		{
			html_add_tag(&div, \
				"<a href='javascript:"\
				"createCookie(\"module\",\"", \
				x=split_to_xstring(i,URL_CHARS,6,2) \
				,"\",\"7\"),get_url(\"\")' " \
				"style='background:#aa2211;color:#FFF;margin-left:5px;'"\
				"><div style='margin:1px 10px;display: inline-block;'>");
				
			nfree(x);

			html_add_tag(&div, \
				Modules[i].name, \
				NULL, "</div></a>" );
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
			retcode=Modules[client_request->module].func( \
				Modules[client_request->module].data, \
				client_request);
		}
	}

	//LOGOUT BUTTON
	if(retcode==0)
	{
		html_add_tag(&html->header, \
			"<a href='javascript:"\
			"eraseCookie(\"login\"),get_url(\"\")' " \
			"style='background:#aa2211;color:#FFF;margin-right:5px;width:9%;vertical-align: top;'"\
			"><div style='margin:1px 10px;display: inline-block;'>", \
			"Logout","</div></a>");
	}
	return retcode;
	
	ERROR_SERVER:
		x=html_flush(&html->base,1);
		nfree(x);
		if(!retcode)html_add_tag(&html->base, HTTP_451, "", "");
		return 2;
}
