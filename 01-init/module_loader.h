#ifndef __MODULE_LOADER_H__
#define __MODULE_LOADER_H__

#include "../utils/lwutils.h"

/**
 * \brief Close dynamically loaded modules
 */
void *dl_unload(void *a);

/**
 * \brief Close lua based modules
 */
void *lua_unload(void *a);

/**
 * \brief Load module varName
 */
void load_module(const char *varName, int x);

/**
 * \brief Answer request in lua
 */
int lua_answer_request(void *md, request *client_request);

#endif
