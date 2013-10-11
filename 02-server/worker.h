#ifndef __WORKER_H__
#define __WORKER_H__

#include "../utils/lwutils.h"

extern int send_login(char **buffer);

extern void *workerthread();

#endif
