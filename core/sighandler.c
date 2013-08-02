#include "lunkwill.h"

/** \brief Adds a new functionn to the queue.
 * The function will be executed on SIGINT or SIGTERM */
void sighndlr_add(void *(*func)(void *), void *param)
{
	sighndlr_list *sig_list;

	sig_list=(sighandler)->next;
	while(sig_list!=NULL){
		sig_list=sig_list->next;
	}

	sig_list=calloc(sizeof(sighndlr_list),1);	
	sig_list->func=func;
	sig_list->param=param;
}

/** \brief Removes a new functionn from the queue. */
void sighndlr_remove(void *(*func)(void *), void *param)
{
	sighndlr_list *sig_list, *sig_prev;

	sig_prev=sighandler;
	sig_list=(sighandler)->next;
	while(sig_list!=NULL){
		if(sig_list->func==func && sig_list->param==param){
			sig_prev->next=sig_list->next;
			nfree(sig_list);
		}
		sig_prev=sig_list;
		sig_list=sig_list->next;
	}
}


/** \brief Works throu queue. */
void sighndlr_safe_exit(int param)
{
	sighndlr_list *sig_list, *n;
	sig_list=sighandler;
	printf("Please wait ");
	while(1){
		printf(".");
		if(sig_list!=NULL){
			if(sig_list->func!=NULL){
				sig_list->func(sig_list->param);
			}
		}
		else
		{
			printf("\n");
			exit(0);
		}
		n=sig_list;
		sig_list=sig_list->next;
		nfree(n);
	}
}

/** \brief Set up signalhandlers */
void init_sighndlr()
{
	sighandler=calloc(sizeof(sighndlr_list),1);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGINT, sighndlr_safe_exit);
	signal(SIGTERM, sighndlr_safe_exit);
}
