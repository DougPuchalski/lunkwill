#include "sighandler.h"

void sighndlr_add(void *(*func)(void *), void *param)
{
	sighndlr_list *sig_list;
	sig_list=calloc(sizeof(sighndlr_list),1);	
	sig_list->func=func;
	sig_list->param=param;

	fifo_push(&sighandler, sig_list);
}

void atexit_safe_exit()
{
	sighndlr_list *sig_list;
	printf("Please wait ");
	while((sig_list=fifo_pop(&sighandler))!=NULL){
		printf(".");
		if(sig_list->func!=NULL){
			sig_list->func(sig_list->param);
		}
		nfree(sig_list);
	}
	printf("\n");

	#ifndef NO_DBG
		if(stddebug!=NULL)
		{
			printf("\x1b[0;0m");
			fclose(stddebug);
			stddebug=NULL;
		}
	#endif
}

void sighndlr_safe_exit(int param)
{
	exit_server=1;
}

void init_sighndlr()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGINT, sighndlr_safe_exit);
	signal(SIGTERM, sighndlr_safe_exit);
	atexit(atexit_safe_exit);
}
