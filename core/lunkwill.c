#include "lunkwill.h"
#include "server.h"
#include "config.h"
#include "../modules/modules.h"

struct _fifo *sighandler=NULL;
struct module_info modules[256];
FILE *stddebug;


int main(int argc, char** argv)
{
	char *config_path=NULL;
	char *err=NULL;
    int pipe1[2];
    #define CHILD_READ pipe1[0]
    #define PARENT_WRITE pipe1[1]
    int pipe2[2];
    #define PARENT_READ pipe2[0]
    #define CHILD_WRITE pipe2[1]
	int opt;
    config_t config;
    pid_t pid;

#ifndef NO_DBG
	if(fcntl(3, F_GETFL) == -1 || errno == EBADF)
	{
		dup2(2,3);
	}
	stddebug=fdopen(3, "w");
#endif
	
	memset(modules, 0, 256*sizeof(struct module_info));
	
	//Parse args
	while((opt=getopt(argc,argv,"c:"))!=-1)
	{
		switch(opt)
		{
			case 'c':
				config_path=malloc(strlen(optarg)+10);
				strcpy(config_path, optarg);
			break;
			default:
				err=argv[0];
				goto argv_fail;
			break;
		}
	}

	//Load configuration
	if(config_path!=NULL)
	{
		printf("INITIALIZING LUNKWILL");
		printf("USING CONFIG: %s\n", config_path);
		if(load_config(&config, config_path)!=0)
		{
			err="Failed to load configuration";
			nfree(config_path);
			goto _fail;
		}
		nfree(config_path);
	}
	else
	{
		printf("INITIALIZING LUNKWILL\n");
		if(load_config(&config,"lunkwill.cfg")!=0)
		{
			printf("CREATING DEFAULT CONFIG\n");
			if(create_config(&config, "lunkwill.cfg")!=0)
			{
				err="Failed to create default configuration";
				goto _fail;
			}
		}
		printf("USING DEFAULT CONFIG\n");
	}

	if(!init_db())
	{
		err="Failed to load database";
		goto _fail;
	}
	
	init_sighndlr();
	fflush(stdout);
	
	if (pipe(pipe1) == 0 && pipe(pipe2) == 0)
	{
		pid = fork();
		if (pid == (pid_t)-1)
		{
			err="Unable to fork worker";
			goto _fail;
		}
		else if (pid == (pid_t)0)
		{
		//Child process
			close(PARENT_READ);
			close(PARENT_WRITE);
			fclose(stdin);

			//Read config
			//modules_init;
			if(login_init_module(__COUNTER__)!=0) return 1;
			if(projects_init_module(__COUNTER__)!=0) return 1;


			int max_num_threads=0, conf;
			config_setting_t *config_prop;
			

			if ((config_prop=config_lookup(&config, "WORKER"))==NULL)
			{
				config_prop=config_root_setting(&config);
			}

			if (config_setting_lookup_int(config_prop, "MAX_NUM_THREADS", &conf)) max_num_threads=conf;
			if(max_num_threads<=0||max_num_threads>0xFFFF) max_num_threads=15;
			config_destroy(&config);
			
			start_worker(max_num_threads, CHILD_READ, CHILD_WRITE);

		}
		else
		{
		//Parent process
            close(CHILD_READ);
            close(CHILD_WRITE);
			
			//Read config
			int conf, port=0;
			int listen_queue=0;
			int timeout=0;
			config_setting_t *config_prop;
			

			if ((config_prop=config_lookup(&config, "SOCKET"))==NULL)
			{
				config_prop=config_root_setting(&config);
			}

			if (config_setting_lookup_int(config_prop, "PORT", &conf))	port=conf;
			if(port<=0||port>0xFFFF) port=3000;

			if (config_setting_lookup_int(config_prop, "PEND_CONNECTIONS", &conf)) listen_queue=conf;
			if(listen_queue<=0||listen_queue>0xFF) listen_queue=20;

			if (config_setting_lookup_int(config_prop, "RCV_TIMEOUT", &conf)) timeout=conf;
			if(listen_queue<=0||listen_queue>0xFF) timeout=1;

			config_destroy(&config);
			
			start_server(port, listen_queue, timeout, PARENT_READ, PARENT_WRITE);
        }
    }
	
	sighndlr_safe_exit(0);
	
	return 0;

	argv_fail:
		fprintf(stderr, "Usage: %s [-c CONFIG_FILE]\n",err);
		return 1;
	
	_fail:
		dbgprintf("%s\n",err);
		return 2;

}
