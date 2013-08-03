#ifndef __SERVER_H__
#define __SERVER_H__

#include "lunkwill.h"

/** \brief This struct contains the parsed request */
typedef struct{
	char special_file; 			/// \brief non_special=0, index.html, logo.png, favicon.ico
	char session_id[21];
	char project_id[5];
	char module_id[3];
	char module_request[BUFSIZ];
} request; 

#endif
