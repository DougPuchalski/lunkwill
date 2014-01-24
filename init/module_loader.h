#ifndef __MODULE_LOADER_H__
#define __MODULE_LOADER_H__

#include <dlfcn.h>
#include <pthread.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "../utils/lwutils.h"
#include "../utils/log.h"

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

struct lua_data
{
	lua_State *L;
	pthread_mutex_t lua_lock;
};

#endif
