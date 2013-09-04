#ifndef __MODULES_H__
#define __MODULES_H__

#include "../core/lunkwill.h"
#include "../core/sighandler.h"
#include "../core/html.h"

#include "login.h"
#include "projects.h"

/** \brief Contains all important module infos*/
struct module_info{
	int id;
	char *name;
	char *description;
	int ((*func)(void *module_data, request *client_request));
	void *data;
};

/** \brief List of all Modules*/
extern struct module_info modules[256];

#endif
