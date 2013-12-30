#ifndef __HTTP_H__
#define __HTTP_H__

#include "../utils/lwutils.h"

/** \brief Supported MIME types */
enum mime_types
{
    MIME_UNKNOWN=0,
    MIME_PLAINTEXT,
    MIME_HTML,
    MIME_JAVASCRIPT,
    MIME_CSS,
    MIME_PNG
};

/** \brief Fills the buffer with an HTTP file request answer
 *  \returns The size of the answer to send */
extern int send_file(char **buffer, char *file_path);

/** \brief Get the MIME Type
 *  \returns A mime_types value */
extern int get_mime(char *file_path);

/** \brief Fills the buffer with an HTTP answer containing a string
 *  \returns The size of the answer to send */
extern int send_string(char **buffer, char *string);

/** \brief Copy string new allocated string, e.g. for sending error message
 *  \returns The size of the answer to send */
extern int send_raw(char **buffer, char *string);

#endif
