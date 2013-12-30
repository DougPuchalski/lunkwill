#include "login.h"
#include "gen_user.h"

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
	pthread_mutex_unlock(&md->search_lock);

	//~ parse_logins(md);
	unsigned char *demo_user = (unsigned char *)"root@localhost.lo";
	unsigned char *demo_pw = (unsigned char *)"toor";

	generate_user(md, demo_user, demo_pw);

	md->site="<h3>Account</h3>";

	sighndlr_add(login_close_module, md);

	return 0;
}


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

	char *passwd_content = calloc(1, passwd_fs);
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
		unsigned char pw[21] = {0};
		memcpy(pw, &passwd_content[i+100], 20);

		if(add_string(md->search, (unsigned char *)&passwd_content[i], pw) != 0)
		{

			pthread_mutex_unlock(&md->search_lock);

			log_write("Error on writing passwd data into search tree", LOG_ERR, 0);
			fclose(passwd);
			free(passwd_content);

			return 1;
		}
	}
	pthread_mutex_unlock(&md->search_lock);

	free(passwd_content);
	fclose(passwd);

	return 0;
}

int check_user_password(struct login_data* md, char *user, char *password)
{
	// Hash the given password
	unsigned char hashed_password[20];
	SHA1((unsigned char *)password, strlen(password), hashed_password);

	pthread_mutex_lock(&md->search_lock);

	if(check_string(md->search, hashed_password, user)!=0)
	{
		pthread_mutex_unlock(&md->search_lock);
		log_write("", LOG_ERR, 1, "User %s tried to login with invalid password", user);
		return 1;
	}

	pthread_mutex_unlock(&md->search_lock);

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
		// Check login data
			char *login_decoded = b64_decode(client_request->module_request, B64_DEFAULT);
			char *delimiter_ptr;

			delimiter_ptr = strstr(login_decoded, "@##@");
			if(delimiter_ptr == NULL)
			{
				log_write("Invalid module data", LOG_DBG, 0);
				goto LOGIN;
			}

			delimiter_ptr[0]=0;
			char *username=login_decoded;
			char *password=delimiter_ptr+4;


			if(check_user_password(module_data, username, password) == 0)
			{
				html_add_tag(&html->main, "<script>", "window.setCookie('login', 'BBBBBBBBBBBBBBBBBBBB');","</script>");
				free(login_decoded);
				return 0;
			}
			free(login_decoded);

	LOGIN:
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
