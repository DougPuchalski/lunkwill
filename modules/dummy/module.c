/** \file module.c
 *  \brief A sample dummy module
 */

#include "module.h"

/** \brief Initializes the module */
char init_module(void){
	printf("Initialized '%s' by %s\n", MODULE_NAME, MODULE_AUTHOR);
	
	// Init successfull!
	return 1;
}


/** \brief Parses get requests */
char *get_request(char *request){
	char *answer = malloc(BUFSIZ);
	char *http_answer = malloc(BUFSIZ+100);
	
	sprintf(answer, "Hello %s. Greetings out of the dummy module :)\n", request);
	send_string(http_answer, answer);
	
	return http_answer;	
}


/** \brief Exits dummy module */
char close_module(void){
	// Closed successfully
	return 1;
}
