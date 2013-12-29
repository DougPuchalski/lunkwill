#include "gen_user.h"

int generate_user(node *tree, unsigned char *username, unsigned char *password)
{
	unsigned char hashed_password[20];
	SHA1(password, strlen(password), hashed_password);

	FILE *user_db;
	if((fopen("user.db", "a")) == NULL)
	{
		log_write("Couldn't create user", LOG_DBG, 0);
		return 1;
	}

	unsigned char new_user[100+21];
	strncpy(new_user, username, 99);
	memcpy(new_user+101, hashed_password, 20);

	fwrite(new_user, 1, 121, user_db);
	fclose(user_db);

	// Add user to tree in RAM!
	if(add_string(tree, password, username) != 0)
	{
		log_write("Couldn't create user", LOG_DBG, 0);
		return 1;
	}

	return 0;
}
