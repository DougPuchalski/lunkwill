#include "gen_user.h"

int generate_user(struct login_data *md, unsigned char *username, unsigned char *password)
{
	unsigned char hashed_password[20];
	SHA1(password, strlen((char *)password), hashed_password);

	FILE *user_db;
	if((user_db = fopen("user.db", "a")) == NULL)
	{
		log_write("Couldn't create user", LOG_DBG, 0);
		return 1;
	}

	unsigned char new_user[100+21];
	strncpy((char *)new_user, (char *)username, 99);
	memcpy(new_user+101, hashed_password, 20);

	fwrite(new_user, 1, 121, user_db);
	fclose(user_db);

	// Add user to tree in RAM!
	pthread_mutex_lock(&md->search_lock);
	if(add_string(md->search, password, username) != 0)
	{
		log_write("Couldn't create user", LOG_DBG, 0);
		return 1;
	}
	pthread_mutex_unlock(&md->search_lock);

	return 0;
}
