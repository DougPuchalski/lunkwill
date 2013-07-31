/** \file git.c
 *  \brief A lunkwill git module
 */

#include "../modules.h"

void *git_close_module(void *arg);
int git_get_request(void *request, struct html_ui* user_iface);

/** \brief Initializes the module */
char git_init_module(int id)
{
	printf("GIT MODULE INITIALIZED AT: /%d\n", id-1);
	session.module[id]=git_get_request;
	sighndlr_add(git_close_module, NULL);
	
	modules[id].id=id;
	modules[id].name="git";
	modules[id].description="A simple git module";
	
	return 1;
}

/** \brief Parses get requests */
int git_get_request(void *request, struct html_ui* user_iface)
{
	void *html_ptr;

	html_ptr=html_add_tag( \
		&user_iface->sidebar, \
		"<table>", NULL, "</table>");
	
	html_add_tag( \
		&html_ptr, \
		"<tr><td>", "master", "</td></tr>");
	
	html_add_tag( \
		&user_iface->main, \
		"<h1>", "git", "</h1><br>");

	html_add_tag( \
		&user_iface->main, \
		"", "We'll show the logs here", "");

	return 0;	
}


/** \brief Exits git module */
void *git_close_module(void *arg){
	// Closed successfully
	return NULL;
}

