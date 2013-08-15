#ifndef __SERVER_H__
#define __SERVER_H__

enum SPECIAL_FILE{
	NON_SPECIAL=0,
	INDEX_HTML,
	LOGO_PNG,
	FAVICON_ICO,
	LINK_RESOLVER,
	ERROR_451=0XFF,
};

/** \brief This struct contains the parsed request */
typedef struct{
	enum SPECIAL_FILE special_file;
	char session_id[21];
	int user;
	int group;
	int session1;
	int session2;
	char project_id[5];
	int project;
	char module_id[3];
	int module;
	char module_request[BUFSIZ];
} request; 

extern int strnmatch(char *a, char *b, int n);

#endif
