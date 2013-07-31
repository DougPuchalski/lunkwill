/** \file module.c
 *  \brief A sample dummy module
 */

#include "../modules.h"

void *dummy_close_module(void *arg);
int dummy_get_request(void *request, struct html_ui* user_iface);

/** \brief Initializes the module */
char dummy_init_module(int id)
{
	printf("DUMMY MODULE INITIALIEZED AT: %d\n",id);
	session.module[id]=dummy_get_request;
	sighndlr_add(dummy_close_module, NULL);
	
	modules[id].id=id;
	modules[id].name="Dummy";
	modules[id].description="Only for debuging";
	
	return 1;
}

/** \brief Parses get requests */
int dummy_get_request(void *request, struct html_ui* user_iface)
{
	void *html_ptr;

	html_ptr=html_add_tag( \
		&user_iface->sidebar, \
		"<table>", NULL, "</table>");
	
	html_add_tag( \
		&html_ptr, \
		"<tr><td>", "Dummy Listing1", "</td></tr>");

	html_add_tag( \
		&html_ptr, \
		"<tr><td>", "Dummy Listing2", "</td></tr>");
	
	html_add_tag( \
		&user_iface->main, \
		"<h1>", "Dummy title in mainfield", "</h1><br>");

	html_add_tag( \
		&user_iface->main, \
		"", "Dummy text in mainfield", "");

	return 0;	
}


/** \brief Exits dummy module */
void *dummy_close_module(void *arg){
	// Closed successfully
	return NULL;
}
