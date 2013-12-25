#include "sighandler.h"

void *sighndlr_free(void *a)
{
	nfree(a);
	return NULL;
}

void sighndlr_add(void *(*func)(void *), void *param)
{
	sighndlr_list *sig_list;
	sig_list=calloc(sizeof(sighndlr_list),1);	
	sig_list->func=func;
	sig_list->param=param;

	fifo_push(&Sighandler, sig_list);
}

void atexit_safe_exit()
{
	sighndlr_list *sig_list;
	while((sig_list=fifo_pop(&Sighandler))!=NULL)
	{
		//~ printf(".");
		if(sig_list->func!=NULL)
		{
			sig_list->func(sig_list->param);
		}
		nfree(sig_list);
	}
	log_write("Server stopped", LOG_WARN, 0);
}

void sighndlr_safe_exit(int param)
{
	Exit_Server=1;
}

void init_sighndlr()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGINT, sighndlr_safe_exit);
	signal(SIGTERM, sighndlr_safe_exit);
	atexit(atexit_safe_exit);
}
