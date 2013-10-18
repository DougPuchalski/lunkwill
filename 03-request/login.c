#include "login.h"

void *login_close_module(void *arg)
{
	nfree(arg);
    return NULL;
}

int login_init_module(int id)
{
	char buf[64];
	sprintf(buf, "Login module intialized at %d", id);
	log_write(buf, ERRORLEVEL_DEBUG);
	
	Modules[id].id=id;
	Modules[id].name="Account";
	Modules[id].func=login_request;
	Modules[id].description="Account settings";
	struct login_data* md=Modules[id].data=malloc(sizeof(struct login_data));

	//init db here
	md->site="Account";

	sighndlr_add(login_close_module, md);

	return 0;
}

int login_verify(int uid, int gid, int ses1, int ses2 )
{
	//ask db here
	if(uid!=gid&&ses1==ses2)
	{
		return 0;
	}
	return 1;
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

int login_request(void *module_data, request *client_request)
{
	struct html_ui *html=client_request->answer;
	void *x;
	
	if(client_request->user==0)
	{
		if(client_request->group==0 && client_request->session1==0 && \
			client_request->session2==0 && client_request->module_request[0]!=0)
		{
			DBGPRINTF("[LOGIN] %s\n", client_request->module_request);
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
	
	if(client_request->answer!=NULL)
	{
		html_add_tag(&((struct html_ui*)client_request->answer)->main,NULL,((struct login_data*)module_data)->site,NULL);
		return 0;
	}

	if(!login_verify(client_request->user, \
		client_request->group, client_request->session1, \
		client_request->session2)) goto ERROR_SERVER;
		
	//MODULE LIST
	int i;
	for(i=0;i<256;i++)
	{
		if(Modules[i].name!=NULL)
		{
			html_add_tag(&html->header, \
				"<a href='javascript:"\
				"createCookie(\"module\",\"", \
				x=split_to_xstring(i,URL_CHARS,6,2) \
				,"\",\"7\"),get_url(\"\")' " \
				"style='background:#aa2211;color:#FFF;margin-left:5px;'"\
				"><div style='margin:1px 10px;display: inline-block'>");
				
			nfree(x);

			html_add_tag(&html->header, \
				Modules[i].name, \
				NULL, "</div></a>" );
		}
	}

	//LOGOUT BUTTON
	html_add_tag(&html->header, \
		"<a href='javascript:"\
		"eraseCookie(\"login\"),get_url(\"\")' " \
		"style='background:#aa2211;color:#FFF;"\
		"position:absolute;right:0px;'" \
		">","<div style='margin:1px 10px;'>Logout</div>","</a>");

	//CALL MODULE
	if(Modules[client_request->module].func!=NULL)
	{
		Modules[client_request->module].func( \
			Modules[client_request->module].data, \
			client_request);
	}

	return 0;
	
	ERROR_SERVER:
		html_add_tag(&html->main, HTTP_451, "", "");
		return 2;
}
