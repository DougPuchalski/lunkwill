#ifndef __LOG_H__
#define __LOG_H__

#include "lwutils.h"

#ifdef CONSOLE_LOG
	#define log_write(message, error_level) logprint(message, error_level, 1)
#else
	#define log_write(message, error_level) logprint(message, error_level, 0)
#endif

enum {LOG_DBG, LOG_INFO, LOG_WARN, LOG_ERR, LOG_FATAL};

/** \brief Write to the lunkwill logfile and to stderr
 *  \returns 0 on success; 1 otherwise */
extern int logprint(char *message, int error_level, int print_stderr);
extern int init_logger(char *LOGFILE, int log_lev);
extern void *close_log(void *a);

#endif
