#ifndef __PROJECTS_H__
#define __PROJECTS_H__

#include "../../utils/lwutils.h"

/** \brief Data passed to all request calls*/
struct projects_data
{
	int i;
};

/** \brief Exits module */
extern void *projects_close_module(void *arg);

/** \brief Callback function for sqlite3_exec. Prints the projects */
extern int print_projects(void *data, int num, char **arguments, char **colnames);

/** \brief Initalizes module */
extern int projects_init_module(struct module_info *m);

/** \brief Request handler */
extern int projects_request(void *md, request *client_request);

#endif
