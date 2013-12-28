#ifndef __REQUEST_PARSER_H__
#define __REQUEST_PARSER_H__

#include "../utils/lwutils.h"

/** \brief Used in requests to know, which type of file is requested, and what type the answer consists of */
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
