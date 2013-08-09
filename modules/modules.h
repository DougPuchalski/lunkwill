
#ifndef __MODULES_H__
#define __MODULES_H__

#include "../core/lunkwill.h"


extern int init_modules();

struct{
	int id;
	char *name;
	char *description;
} modules[63];

#endif
