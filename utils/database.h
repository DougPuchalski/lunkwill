#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "../utils/lwutils.h"

/** \brief Function to satisfy sqlite
 *  \returns Always 0 */
extern int silly_sqlite_callback(void *notused, int notusedeither, char **notusedeither2, char **notusedeither3);

/** \brief Initializes the database */
extern int init_db(void);

/** \brief Closes the database */
extern void *close_db(void *null);

#endif
