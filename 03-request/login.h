#ifndef __LOGIN_H__
#define __LOGIN_H__

#include "../utils/lwutils.h"

/** \brief Struct which is passed to every call */
struct login_data
{
	int i;
	char *site;
};

/** \brief Exits login module */
extern void *login_close_module(void *arg);

/** \brief Initializes the login module */
extern int login_init_module(int id);

/** \brief Verify user and session */
extern int login_verify(int uid, int gid, int ses1, int ses2 );

/** \brief Create a new session */
extern int login_new_session(char *input, int uid, int gid, int ses1, int ses2 );

/** \brief Handlerfunction for requests */
extern int login_request(void *module_data, request *client_request);

#endif
