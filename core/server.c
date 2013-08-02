#include "lunkwill.h"

/** \brief Threads which handles client request */
void *client_trhead(void * arg)
{
	unsigned int client_sock;
	char recv_buf[BUF_SIZE];
	char *file_name=NULL;
	char *send_buf=NULL;
	void *leek;
	int msg_len;
	struct html_ui *user_iface=new_html();
 
	client_sock = *(unsigned int *)arg;

	if(recv(client_sock, recv_buf, BUF_SIZE, 0)<0)
	{
		exit(NULL);
	}
	else
	{
		//Search HTTP GET method
		file_name=strstr(recv_buf, "GET /");
		if(file_name==NULL)
		{
			goto HTTP404;
		}
		else
		{
			file_name=&file_name[5];
		}

		//Initialize list of modules
		if(session.module[0]!=NULL)
		{
			if(session.module[0](file_name,user_iface)!=0) goto HTTP404;
		}
		else goto HTTP404;
		
		if(file_name[1]!='/' && file_name[0]!=' ' && file_name[0]!='%') goto HTTP404;
		
		/* Open module
		 * Modules are identified with a single char [0-9a-zA-Y]
		 * Char 'Z' allows to use the server as a normal http server
		 * */ 
		if(file_name[0]>='0'&&file_name[0]<='9')
		{
			if(session.module[file_name[0]-'0'+1]!=NULL)
			{
				if(session.module[file_name[0]-'0'+1](file_name, user_iface)!=0) goto HTTP404;
			}
			else goto HTTP404;			
		}
		else if(file_name[0]>='a'&&file_name[0]<='z')
		{
			if(session.module[file_name[0]-'a'+11]!=NULL)
			{
				if(session.module[file_name[0]-'a'+11](file_name, user_iface)!=0) goto HTTP404;
			}
			else goto HTTP404;			
		}
		else if(file_name[0]>='A'&&file_name[0]<='Y')
		{
			if(session.module[file_name[0]-'A'+37]!=NULL)
			{
				if(session.module[file_name[0]-'A'+37](file_name, user_iface)!=0) goto HTTP404;
			}
			else goto HTTP404;			
		}
		else if(file_name[0]=='Z'||file_name[0]==' ') 
		{
			//Open the file for sending.
			//Request "GET / " is interpreted like "GET /Z/index.html"

			char path[BUF_SIZE<<5];
			char *tmp;

			if (file_name[0]==' ')
			{
				strcpy(file_name+1,"index.html");
			}
			else
			{
				int i;
				for(i=0;i<BUF_SIZE;i++)
				{
					if(file_name[i]==' ')
					{
						file_name[i]=0;
						break;
					}
				}
			}
			
			if (config_lookup_string(&session.config, "WWW_ROOT", (const char**)&tmp))
			{
				sprintf(path,"%s/%s", tmp, &file_name[1]);
			}
			else
			{
				sprintf(path,"%s/%s", "www", &file_name[1]);
			}
			
			if((msg_len=send_file(&send_buf, path))>0)
			{
				nfree(user_iface);
				goto SEND;
			}
			else
			{
				nfree(send_buf);
				goto HTTP404;
			}
		}
		else
		{
			
			html_add_tag( \
				&user_iface->main, \
				"<h1>", "lunkwill", "</h1><br>");

			html_add_tag( \
				&user_iface->main, \
				"", "lunkwill is a lightweight bug tracking and project management tool.", "");
		}
	}

	if(send_string(&send_buf, (leek=html_flush(&user_iface->base, 0)))<0)
	{
		nfree(leek);
		goto HTTP404;
	}
	nfree(leek);
	msg_len=strlen(send_buf);

	SEND:
		send(client_sock, send_buf, msg_len, 0);
		nfree(send_buf);
		nfree(user_iface);
		close(client_sock);
		exit(NULL);
	
	HTTP404:
		nfree(user_iface);
		send(client_sock, HTTP_404, strlen(HTTP_404), 0);
		close(client_sock);
		exit(NULL);
}

/** \brief Server which spawns threads for all clients */
int start_server()
{
	unsigned int server_sock, client_sock, addr_len;
	struct sockaddr_in server_addr, client_addr;
	int con=0,port=0;
	int conf;
 
//	pthread_attr_t attr;
//	pthread_t threads;
//	pthread_attr_init(&attr);

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
//			pthread_create (
//				&threads, 
//				&attr,    
//				client_trhead,
//				&client_sock);
			pid_t pid;

			fflush(stdout);

			pid=fork();
			if(pid<0)
			{
				exit(1);
			}
			if(pid!=0)
			{
				close(client_sock);
			}
			else
			{
				close(server_sock);
				client_trhead(&client_sock);
				exit(0);
			}
		}
	}
	close(server_sock);
	return 1;
}
