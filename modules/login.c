#include "login.h"

void *login_close_module(void *arg)
{
	//~ dbgprintf("Login database closed\n%s","");
    return NULL;
}

int login_init_module(int id)
{
	printf("LOGIN MODULE INITIALIZED AT: %d\n", id);
	modules[id].id=id;
	modules[id].name="Account";
	modules[id].func=login_request;
	modules[id].description="Account settings";
	struct login_data* md=modules[id].data=malloc(sizeof(struct login_data));

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
	//~ struct login_data *l=module_data;
	
	if(client_request->user==0)
	{
		if(client_request->group==0 && client_request->session1==0 && \
			client_request->session2==0 && client_request->module_request[0]!=0)
		{
			dbgprintf("[LOGIN] %s\n", client_request->module_request);
			if((login_new_session((char*)client_request->module_request, client_request->user, \
				client_request->group, client_request->session1, \
				client_request->session2))!=0)goto ERROR_SERVER;
			return 3;
		}
		return 1;
	}
	
	if(client_request->answer==NULL)
	{
		return login_verify(client_request->user, \
			client_request->group, client_request->session1, \
			client_request->session2);
	}
	
	html_add_tag(&((struct html_ui*)client_request->answer)->main,NULL,((struct login_data*)module_data)->site,NULL);
	return 0;
	
	ERROR_SERVER:
		return 2;
}
