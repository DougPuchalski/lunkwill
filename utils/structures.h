#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

/** \brief This struct contains the parsed request */
typedef struct
{

	char session_id[21];
	char project_id[5];
	char module_id[3];
	char module_request[BUF_SIZE];

	int special_file;
	int user;
	int group;
	int session1;
	int session2;
	int project;
	int module;

	void *answer;

} request;

/** \brief Contains all important module infos*/
struct module_info
{
	int id;
	char *name;
	char *description;
	int ((*func)(void *module_data, request *client_request));
	void *data;
};

/** \brief List of all Modules*/
extern struct module_info Modules[256];

#endif
