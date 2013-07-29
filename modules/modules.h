#ifndef __MODULES_H__
#define __MODULES_H__

#include "../core/lunkwill.h"

#include "dummy/module.h"
#include "tickets/module.h"
#include "projects/module.h"
#include "source/module.h"
#include "wiki/module.h"

extern int init_modules();

EXTERN struct{
	int id;
	char *name;
	char *description;
} modules[63];

#endif
