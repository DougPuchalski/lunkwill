#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "lwutils.h"
#include "sighandler.h"
#include "fifo.h"

#define LOG_LEV_UNDEF 666

#ifdef CONSOLE_LOG
#define log_write(message, error_level, ...) logprint(init_logger(NULL, LOG_LEV_UNDEF), message, error_level, 1, ##__VA_ARGS__)
#else
#define log_write(message, error_level, ...) logprint(init_logger(NULL, LOG_LEV_UNDEF), message, error_level, 0, ##__VA_ARGS__)
#endif

enum {LOG_DBG, LOG_INFO, LOG_WARN, LOG_ERR, LOG_FATAL};

struct logger
{
	int level;
	FILE *file;
};

/** \brief Write to the lunkwill logfile and to stderr
 *  \returns 0 on success; 1 otherwise */
extern int logprint(struct logger log, char *message, int error_level, char print_stderr, ...);
extern struct logger init_logger(char *LOGFILE, int log_lev);
extern void *close_log(void *a);

#endif
