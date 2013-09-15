#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "lunkwill.h"

/** \brief Function to satisfy sqlite
 *  \returns Always 0 */
static int silly_sqlite_callback(void *notused, int notusedeither, char **notusedeither2, char **notusedeither3);

/** \brief Initializes the database */
int init_db(void);

/** \brief Closes the database */
void close_db(void);

#endif
