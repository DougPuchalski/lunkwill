#include "lunkwill.h"

fifo_t *Sighandler=NULL;
struct module_info Modules[256];

/**
 * \brief Where it all begins
 */
int main(int argc, char** argv)
{
	char *err=NULL;
	int pipe1[2];
	int pipe2[2];
	pid_t pid;
	int opt;

	int log_level=0;
	char *log_file=NULL;

	int conf;
	char *config_path=NULL;
	config_t config;
	config_setting_t *config_prop;

	memset(Modules, 0, 256*sizeof(struct module_info));

	//Parse args
	while((opt=getopt(argc,argv,"c:f"))!=-1)
	{
		switch(opt)
		{
		case 'c':
			if(config_path==NULL)
			{
				config_path=malloc(strlen(optarg)+10);
				strcpy(config_path, optarg);
			}
			break;
		case 'f':
			switch((int)fork())
			{
			case -1:
				err="Unable to fork worker";
				nfree(config_path);
				goto _fail;
				break;
			case 0:
				log_level=99;
				break;
			default:
				nfree(config_path);
				return 0;
				break;
			}
			break;
		default:
			err=argv[0];
			nfree(config_path);
			goto argv_fail;
			break;
		}
	}

	//Load configuration
	if(config_path!=NULL)
	{
		log_write("Initializing lunkwill", LOG_DBG);
		log_write("Using config: %s", LOG_DBG, config_path);


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
		log_write("Initializing lunkwill", LOG_DBG);
		if(load_config(&config,"lunkwill.cfg")!=0)
		{
			log_write("Creating default config", LOG_DBG);
			if(create_config(&config, "lunkwill.cfg")!=0)
			{
				err="Failed to create default configuration";
				goto _fail;
			}
		}
		log_write("Using default config", LOG_DBG);
	}

	if((config_prop=config_lookup(&config, "LOG"))==NULL)
	{
		config_prop=config_root_setting(&config);
	}

	if(config_setting_lookup_int(config_prop, "LOGLEVEL", &conf)&&log_level==0) log_level=conf;
	if(!config_setting_lookup_string(config_prop, "LOGFILE", (const char**)&log_file)) return 1;

	init_logger(log_file,log_level);

	init_sighndlr();
	fflush(stdout);

	if(pipe(pipe1) == 0 && pipe(pipe2)==0)
	{
		pid = fork();
		if(pid == (pid_t)-1)
		{
			err="Unable to fork worker";
			goto _fail;
		}
		else if(pid == (pid_t)0)
		{
			//Child process
			close(pipe2[0]);
			close(pipe1[1]);
			fclose(stdin);

			//Read config
			//Modules_init;
			if(login_init_module(0)!=0) return 1;

			int max_num_threads=0;

			if((config_prop=config_lookup(&config, "WORKER"))==NULL)
			{
				config_prop=config_root_setting(&config);
			}

			if(config_setting_lookup_int(config_prop, "MAX_NUM_THREADS", &conf)) max_num_threads=conf;
			if(max_num_threads<=0||max_num_threads>0xFFFF) max_num_threads=15;

			if((config_prop=config_lookup(&config, "MODULES"))==NULL)
			{
				config_prop=config_root_setting(&config);
			}

			int x,n=0;
			n=config_setting_length(config_prop);
			for(x=0; x<n; x++)
			{
				const char *varName;

				if((varName = config_setting_get_string_elem(config_prop, x))==NULL) continue;

				load_module(varName, x);

			}

			config_destroy(&config);

			start_worker(max_num_threads, pipe1[0], pipe2[1]);

		}
		else
		{
			//Parent process
			close(pipe1[0]);
			close(pipe2[1]);

			//Read config
			int conf, port=0;
			int listen_queue=0;
			int timeout=0;
			config_setting_t *config_prop;


			if((config_prop=config_lookup(&config, "SOCKET"))==NULL)
			{
				config_prop=config_root_setting(&config);
			}

			if(config_setting_lookup_int(config_prop, "PORT", &conf))	port=conf;
			if(port<=0||port>0xFFFF) port=3000;

			if(config_setting_lookup_int(config_prop, "PEND_CONNECTIONS", &conf)) listen_queue=conf;
			if(listen_queue<=0||listen_queue>0xFF) listen_queue=20;

			if(config_setting_lookup_int(config_prop, "RCV_TIMEOUT", &conf)) timeout=conf;
			if(timeout<=0||timeout>0xFF) timeout=1;

			config_destroy(&config);
			log_write("Server started. Enter 'quit' to shutdown the server (Port: %d)", LOG_WARN, port);
			start_server(port, listen_queue, timeout, pipe2[0], pipe1[1]);
		}
	}

	sighndlr_safe_exit(0);

	return 0;

argv_fail:
	log_write("Usage: %s [-c CONFIG_FILE] [-f]", LOG_FATAL, err);
	nfree(config_path);
	return 1;

_fail:
	log_write(err, LOG_ERR);
	return 2;

}
