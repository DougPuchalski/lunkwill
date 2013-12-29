#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#include "../utils/lwutils.h"
#include "../03-request/login.h"

/** \brief Generate a user and add him to the user.db file */
extern int generate_user(struct login_data *md, unsigned char *username, unsigned char *password);
