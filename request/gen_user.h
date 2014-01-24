#include "../utils/lwutils.h"
#include "login.h"

/** \brief Generate a user and add him to the user.db file */
extern int generate_user(struct login_data *md, unsigned char *username, unsigned char *password);
