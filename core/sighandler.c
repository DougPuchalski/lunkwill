#include "lunkwill.h"

typedef struct{
	void *(*func)(void *);
	void *param;
	void *next;
} sighndlr_list;


void sighndlr_add(void *(*func)(void *), void *param)
{
	sighndlr_list *sig_list;

	sig_list=((sighndlr_list*)session.sighndlr)->next;
	while(sig_list!=NULL){
		sig_list=sig_list->next;
	}

	sig_list=calloc(sizeof(sighndlr_list),1);	
	sig_list->func=func;
	sig_list->param=param;
}

void sighndlr_remove(void *(*func)(void *), void *param)
{
	sighndlr_list *sig_list, *sig_prev;

	sig_prev=session.sighndlr;
	sig_list=((sighndlr_list*)session.sighndlr)->next;
	while(sig_list!=NULL){
		if(sig_list->func==func && sig_list->param==param){
			sig_prev->next=sig_list->next;
			nfree(sig_list);
		}
		sig_prev=sig_list;
		sig_list=sig_list->next;
	}
}


void sighndlr_safe_exit()
{
	sighndlr_list *sig_list;
	while(1){
		sig_list=session.sighndlr;
		if(sig_list!=NULL){
			if(sig_list->func!=NULL){
				sig_list->func(sig_list->param);
			}
		}else return;
	}
}

void init_sighndlr()
{
	session.sighndlr=calloc(sizeof(sighndlr_list),1);
	signal(SIGINT, sighndlr_safe_exit);
	signal(SIGPIPE, SIG_IGN);
}
