#include "modules.h"

/** \brief Parses get requests */
void *border(void *request){
	char *path,p[BUFSIZ];
	char *http_answer = malloc(BUFSIZ);
	if(((char *)request)[0]==' '){
		if (!config_lookup_string(&session.config, "SERVER_ROOT", (const char **)&path))
		{
			strcpy(p,"www");
		}
		else
		{
			strcpy(p, path);
		}

		strcat(p, "/index.html");

		if(send_file(&http_answer, p, MIME_HTML)==-1)
		{
			return NULL;
		}
	}
	else
	{
		int i;
		char buf[BUFSIZ];

		strcpy(p,"{\"BASE\": {\"HEAD\":{\"LIST\":[");
		for(i=0;i<63;i++){
			if(modules[i].id==i&&modules[i].name!=NULL){
				sprintf(buf,"{\"id\":\"%d\",\"name\":\"%s\",\"description\":\"%s\"}," \
					,modules[i].id-1, modules[i].name, modules[i].description);
				strcat(p,buf);
			}
		}
		p[strlen(p)-1]=0;
		strcat(p,"]}}}");
		send_string(http_answer, p);
	}
	return http_answer;	
}

int init_modules()
{	
	memset(modules,0,sizeof(modules));
	session.module[__COUNTER__]=border;
	dummy_init_module(__COUNTER__);
	projects_init_module(__COUNTER__);
	return 0;
}
