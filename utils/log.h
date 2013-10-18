#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {ERRORLEVEL_DEBUG, ERRORLEVEL_WARN, ERRORLEVEL_ERR, ERRORLEVEL_EMRG};

/** \brief Write to the lunkwill logfile and to stderr
 *  \returns 0 on success; 1 otherwise */
extern int log_write(char *message, int error_level);

#endif
