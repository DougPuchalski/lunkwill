#include "login.h"

void *login_close_module(void *arg)
{
	free_searchtree(((struct login_data*)arg)->search);
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

	pthread_mutex_init(&md->search_lock, NULL);

	pthread_mutex_lock(&md->search_lock);

	md->search = init_searchtree();
	if(md->search == NULL)
	{
		pthread_mutex_unlock(&md->search_lock);
		return 1;
	}

	//	parse_logins(md);

	pthread_mutex_unlock(&md->search_lock);

	md->site="<h3>Account</h3>";

	sighndlr_add(login_close_module, md);

	return 0;
}


// TO BE DOCUMENTED
int parse_logins(struct login_data* md)
{
	FILE *passwd;
	if((passwd = fopen("user.db", "r")) == NULL)
	{
		log_write("Could not read user database", LOG_ERR, 0);
		return 1;
	}

	// Read password file
	int passwd_fs;
	fseek(passwd, 0, SEEK_END);
	passwd_fs = ftell(passwd);
	fseek(passwd, 0, SEEK_SET);

	if(passwd_fs <= 0)
	{
		log_write("Failed on determining size of passwd", LOG_ERR, 0);
		fclose(passwd);

		return 1;
	}

	char *passwd_content = malloc(passwd_fs);
	if(passwd_content == NULL)
	{
		log_write("Failed on allocating passwd memory", LOG_ERR, 0);
		fclose(passwd);

		return 1;
	}

	if((fread(passwd_content, passwd_fs, 1, passwd)) != passwd_fs)
	{
		log_write("Failed on reading passwd", LOG_ERR, 0);
		free(passwd_content);
		fclose(passwd);

		return 1;
	}


	pthread_mutex_lock(&md->search_lock);
	int i;
	for(i=0; i<passwd_fs; i+=120)
	{
		// Copy password to set null terminator
		char pw[21] = {0};
		memcpy(pw, &passwd_content[i+100], 20);

		if(add_string(md->search, &passwd_content[i], pw) != 0)
		{
			log_write("Error on writing passwd data into search tree", LOG_ERR, 0);
			return 1;
		}
	}
	pthread_mutex_unlock(&md->search_lock);

	free(passwd_content);
	fclose(passwd);

	return 0;
}


// TO BE DOCUMENTED
int check_user_password(struct login_data* md, char *user, char *password)
{
	// Hash the given password
	char hashed_password[20];

	// Search the tree for the password
	char *real_user;

	pthread_mutex_lock(&md->search_lock);
	if((real_user = search_string(get_search_tree(), hashed_password)) == NULL)
	{
		pthread_mutex_unlock(&md->search_lock);
		log_write("User %s tried to login with invalid password", LOG_DBG, 1, user);
		return 1;
	}
	pthread_mutex_unlock(&md->search_lock);


	if(strcmp(real_user, user) != 0)
	{
		log_write("Wrong login data: User %s", LOG_DBG, 1, user);
		return 1;
	}

	// User logged in :)
	return 0;
}


int login_verify(int uid, int gid, int ses1, int ses2)
{
	return 0;
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
