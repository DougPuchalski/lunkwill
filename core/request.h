#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "lunkwill.h" //For request

enum SPECIAL_FILE
{
	NON_SPECIAL=0,
	INDEX_HTML,
	LOGO_PNG,
	FAVICON_ICO,
	LINK_RESOLVER,
	ERROR_451=0XFF,
};


/** \brief Parses a GET request 
 *  \param The GET request string to parse
 *  \returns - A pointer to a request request struct which has to be freed by the caller
 * 			 - NULL if the request is not valid
 */  
extern request parse_request(char *get_request);

#endif
