/** \file module.c
 *  \brief A sample dummy module
 */

#include "../modules.h"

void *projects_close_module(void *arg);
char *projects_get_request(char *request);

/** \brief Initializes the module */
char projects_init_module(int id){
	printf("PROJECT MODULE INITIALIEZED AT: %d\n",id);
	session.module[id]=(void *)projects_get_request;
	sighndlr_add(projects_close_module, NULL);
	
	return 1;
}


/** \brief Parses get requests */
char *projects_get_request(char *request){
	char answer[BUFSIZ];
	char *http_answer = malloc(BUFSIZ+100);
	
	sprintf(answer, "Hello %s. Greetings out of the project dummy module :)\n", request);
	send_string(http_answer, answer);
	
	return http_answer;	
}


/** \brief Exits dummy module */
void *projects_close_module(void *arg){
	// Closed successfully
	return NULL;
}
