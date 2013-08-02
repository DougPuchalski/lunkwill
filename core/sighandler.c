#include "lunkwill.h"

extern struct _fifo *sighandler;

/** \brief Adds a new functionn to the queue.
 * The function will be executed on SIGINT or SIGTERM */
void sighndlr_add(void *(*func)(void *), void *param)
{
	sighndlr_list *sig_list;
	sig_list=calloc(sizeof(sighndlr_list),1);	
	sig_list->func=func;
	sig_list->param=param;

	fifo_push(&sighandler, sig_list);
}

/** \brief Works throu queue. */
void sighndlr_safe_exit(int param)
{
	sighndlr_list *sig_list;
	printf("Please wait ");
	while((sig_list=fifo_pop(&sighandler))!=NULL){
		printf(".");
		if(sig_list->func!=NULL){
			sig_list->func(sig_list->param);
		}
		nfree(sig_list->param);
		nfree(sig_list);
	}
}

/** \brief Set up signalhandlers */
void init_sighndlr()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGINT, sighndlr_safe_exit);
	signal(SIGTERM, sighndlr_safe_exit);
}
