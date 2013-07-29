#include "modules.h"

/** \brief Parses get requests */
void *border(void *request){
	char *path,p[BUFSIZ];
	char *http_answer = malloc(BUFSIZ+100);
	
	if (!config_lookup_string(&session.config, "SERVER_ROOT", (const char **)&path))
	{
		strcpy(p,"www");
		path=p;
	}
	
	strcat(path, "/index.html");
	
	if(send_file(http_answer, path, MIME_HTML)==-1)
	{
		free(http_answer);
		return NULL;
	}
	
	return http_answer;	
}

int init_modules()
{	
	session.module[__COUNTER__]=border;
	dummy_init_module(__COUNTER__);
	projects_init_module(__COUNTER__);
	return 0;
}
