#ifndef __SERVER_H__
#define __SERVER_H__

#include "lunkwill.h"

enum SPECIAL_FILE{
	NON_SPECIAL=0,
	INDEX_HTML,
	LOGO_PNG,
	FAVICON_ICO,
	ERROR_404=0XFF,
};

/** \brief This struct contains the parsed request */
typedef struct{
	enum SPECIAL_FILE special_file;
	char session_id[21];
	char project_id[5];
	char module_id[3];
	char module_request[BUFSIZ];
} request; 

#endif
