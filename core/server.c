#include "lunkwill.h"

void *client_trhead(void * arg)
{
	unsigned int client_sock;
	int i;
	char buf[BUF_SIZE];
	char *file_name=NULL;
 
	client_sock = *(unsigned int *)arg;

	if(recv(client_sock, buf, BUF_SIZE, 0)<0)
	{
		pthread_exit(NULL);
	}
	else
	{
		for(i=0;i<BUF_SIZE;i++){
			if(file_name==NULL && buf[i]=='/'){
				file_name=&buf[i+1];
			}
		}

		if(file_name==NULL)
		{
			pthread_exit(NULL);
		} 

		char *tmp=NULL;

		if(session.module[0]!=NULL)
		{
			tmp=session.module[0](file_name);
			if(tmp==NULL) goto HTTP404;
			send(client_sock, tmp, strlen(tmp), 0);
			nfree(tmp);
		}
		else goto HTTP404;			
		
		if(file_name[0]>='0'&&file_name[0]<='9')
		{
			if(session.module[file_name[0]-'0'+1]!=NULL)
			{
				tmp=session.module[file_name[0]-'0'+1](file_name);
				if(tmp==NULL) goto HTTP404;
				send(client_sock, tmp, strlen(tmp), 0);
				nfree(tmp);
			}
			else goto HTTP404;			
		}
		else if(file_name[0]>='a'&&file_name[0]<='z')
		{
			if(session.module[file_name[0]-'a'+11]!=NULL)
			{
				tmp=session.module[file_name[0]-'a'+11](file_name);
				if(tmp==NULL) goto HTTP404;
				send(client_sock, tmp, strlen(tmp), 0);
				nfree(tmp);
			}
			else goto HTTP404;			
		}
		else if(file_name[0]>='A'&&file_name[0]<='Z')
		{
			if(session.module[file_name[0]-'A'+37]!=NULL)
			{
				tmp=session.module[file_name[0]-'A'+37](file_name);
				if(tmp==NULL) goto HTTP404;
				send(client_sock, tmp, strlen(tmp), 0);
				nfree(tmp);
			}
			else goto HTTP404;			
		}
		else goto HTTP404;

	}
	
	close(client_sock);
	pthread_exit(NULL);
	
	HTTP404:
		strcpy(buf, HTTP_404);
		send(client_sock, buf, strlen(buf), 0);
		strcpy(buf, HTTP_404_MESS);
		send(client_sock, buf, strlen(buf), 0);
		close(client_sock);
		pthread_exit(NULL);
}



int start_server()
{
	unsigned int server_sock, client_sock, addr_len;
	struct sockaddr_in server_addr, client_addr;
	int con=0,port=0;
	int conf;
 
	pthread_attr_t attr;
	pthread_t threads;
	pthread_attr_init(&attr);

	if (config_lookup_int(&session.config, "PORT", &conf))
	{
		port=conf;
	}
	if(port<=0||port>0xFFFF){
		port=3000;
	}
	
	if (config_lookup_int(&session.config, "PEND_CONNECTIONS", &conf))
	{
		con=conf;
	}
	if(con<=0||con>0xFF){
		con=50;
	}
	


	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int optval = 1;
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		fprintf(stderr, "Error binding to port\n");
		return 1;
	}

	if((listen(server_sock, con)) < 0){
		fprintf(stderr, "Error listening on port\n");
		return 1;
	}
 

	while(1)
	{
		addr_len = sizeof(client_addr);
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
		if (client_sock != 0)
		{
			pthread_create (
				&threads, 
				&attr,    
				client_trhead,
				&client_sock);
		}
	}
	close(server_sock);
	return 1;
}
