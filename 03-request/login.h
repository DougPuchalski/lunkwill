#ifndef __LOGIN_H__
#define __LOGIN_H__

#include "../utils/lwutils.h"

/** \brief Struct which is passed to every call */
struct login_data
{
	int i;
	char *site;
	void *search;
	pthread_mutex_t search_lock;
};

/** \brief Exits login module */
extern void *login_close_module(void *arg);

/** \brief Initializes the login module */
extern int login_init_module(int id);

/** \brief Verify user and session */
extern int login_verify(int uid, int gid, int ses1, int ses2);

/** \brief Initialize lookuptable for user/password combination */
extern int parse_logins(struct login_data* md);

/** \brief Handlerfunction for requests */
extern int login_request(void *module_data, request *client_request);

/** \brief Return current search tree address */
extern node *get_search_tree(void);

#endif
