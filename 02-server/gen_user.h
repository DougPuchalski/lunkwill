#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "../utils/lwutils.h"

/** \brief Generate a user and add him to the user.db file */
extern int generate_user(node *tree, unsigned char *username, unsigned char *password);
