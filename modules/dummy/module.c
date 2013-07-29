/** \file module.c
 *  \brief A sample dummy module
 */

#include "../modules.h"

void *dummy_close_module(void *arg);
char *dummy_get_request(char *request);

/** \brief Initializes the module */
char dummy_init_module(int id){
	printf("DUMMY MODULE INITIALIEZED AT: %d\n",id);
	session.module[id]=(void *)dummy_get_request;
	sighndlr_add(dummy_close_module, NULL);
	return 1;
}


/** \brief Parses get requests */
char *dummy_get_request(char *request){
	char answer[BUFSIZ];
	char *http_answer = malloc(BUFSIZ+100);
	
	sprintf(answer, "<!DOCTYPE><html><head><title>Dummy Module</title></head><body>Hello! Greetings out of the dummy module :)\n\n<b>Request:</b><br><br>%s</body></html>", request);
	send_string(http_answer, answer);
	return http_answer;	
}


/** \brief Exits dummy module */
void *dummy_close_module(void *arg){
	// Closed successfully
	return NULL;
}
