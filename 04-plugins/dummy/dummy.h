#ifndef __DUMMY_H__
#define __DUMMY_H__

#include "../../utils/lwutils.h"

/** \brief Data passed to all request calls*/
struct module_data
{
	char null;
};

/** \brief Initalizes module. Use "init_module" as fuction name!
 * Only this function gets directly called! */
extern int init_module(int id, struct module_info *m);

/** \brief Exits module */
extern void *close_module(void *arg);

/** \brief Request handler */
extern int answer_request(void *md, request *client_request);

#endif
