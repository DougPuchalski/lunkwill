#ifndef __SERVER_H__
#define __SERVER_H__

#include "lunkwill.h"

typedef struct{
	char session_id[21];
	char project_id[5];
	char module_id[3];
	char module_request[BUFSIZ];
} request; 

#endif
