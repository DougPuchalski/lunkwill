#define __MAIN__
#include "lunkwill.h"

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
				if(config!=NULL) nfree(config);
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
		printf("USING DEFAULT CONFIG\n");
		if(load_config("default.conf")!=0)
		{
			err="Failed to load configuration";
			goto _fail;
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

