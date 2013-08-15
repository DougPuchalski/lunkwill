
#ifndef __MODULES_H__
#define __MODULES_H__

#include "../core/lunkwill.h"


extern int init_modules();
extern int login_init_module(int id);
extern int login_request(void *module_data, request *client_request);

struct module_info{
	int id;
	char *name;
	char *description;
	int ((*func)(void *module_data, request *client_request));
	void *data;
};

extern struct module_info modules[256];

#endif
