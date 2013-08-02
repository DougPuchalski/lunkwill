#define __MAIN__
#include "lunkwill.h"

#define  DEFAULT_CONFIG "PORT = 3000\n\
PEND_CONNECTIONS = 50\n"


int main(int argc, char** argv)
{
	int opt;
	char *config=NULL;
	char *err;
	
	memset(&session, 0, sizeof(session));

	init_sighndlr();

	while((opt=getopt(argc,argv,"c:"))!=-1)
	{
		switch(opt)
		{
			case 'c':
				config=malloc(strlen(optarg)+10);
				strcpy(config, optarg);
			break;
			default:
				err=argv[0];
				goto argv_fail;
			break;
		}
	}

	if(config!=NULL)
	{
		printf("INITIALIZING LUNKWILL");
		printf("USING CONFIG: %s\n", config);
		if(load_config(config)!=0)
		{
			err="Failed to load configuration";
			goto _fail;
		}
	}
	else
	{
		printf("INITIALIZING LUNKWILL\n");
		if(load_config("default.conf")!=0)
		{
			// Write default configuration
			FILE *default_config;
			if( (default_config = fopen("default.conf", "w")) == NULL)
			{
				err="Could not write and load default configuration";
				goto _fail;
			}
			else
			{
				printf("WRITING DEFAULT CONFIG\n");
				fwrite(DEFAULT_CONFIG, strlen(DEFAULT_CONFIG), 1, default_config);
				fclose(default_config);
			}
			
			// Try again
			if(load_config("default.conf") != 0)
			{
				err="Not sure wtf happended here: Couldn't load just written config. Permissions?";
				goto _fail;
			}
		}
		else
		{
			printf("USING DEFAULT CONFIG\n");	
		}
	}

	init_modules();

	if(start_server()!=0)
	{
		err="The server returned an error";
		goto _fail;
	}

	return 0;

	argv_fail:
		fprintf(stderr, "Usage: %s [-c CONFIG_FILE]\n",err);
		return 1;
	
	_fail:
		fprintf(stderr, "%s\n",err);
		return 2;

}

