#include "server.h"


/** \brief Parses a GET request 
 *  \param The GET request string to parse
 *  \returns - A pointer to a request request struct which has to be freed by the caller
 * 			 - NULL if the request is not valid
 * 			 - A pointer to an empty request struct which has to be freed by the caller if index.html is requested 
 */  
request *parse_request(char *get_request)
{
	request *req = malloc(sizeof(request));
	if(req == NULL)
		return NULL;

	
	// Invalid request
	if(strstr(get_request, "GET /") != get_request)
		goto HTTP404;


	// Send index.html
	if(strstr(get_request+5, " ") == get_request+5)
		goto EMPTY;


	// Check session id
	char *ptr = strstr(get_request+5, "/");
	
	// Invalid session id
	if((ptr - (get_request+5)) != 20)
		goto HTTP404;

	// Read session id
	strncpy(req->session_id, get_request+5, 20);

	
	// Check project id
	ptr = strstr(get_request+26, "/");
	
	// Invalid project id
	if((ptr - (get_request+26)) != 4)
		goto HTTP404;

	// Read project id
	strncpy(req->project_id, get_request+26, 4);

	
	// Check module id
	ptr = strstr(get_request+31, "/");
	
	// Invalid module id
	if(ptr - (get_request+31) != 2)
		goto HTTP404;

	strncpy(req->module_id, get_request+31, 2);

	// Find end of module_request
	ptr = strstr(get_request+34, " ");
	if(ptr - (get_request+34) < BUFSIZ-1)
		strncpy(req->module_request, get_request+34, ptr - (get_request+34));
	else
		strncpy(req->module_request, get_request+34, BUFSIZ-1);

	return req;

	
	// Returns empty req struct
	EMPTY:
		memset(req, 0x30, sizeof(req));
		return req;	
	
	// Returns NULL
	HTTP404:
		free(req);
		return NULL;
}


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
		raise(SIGTERM);
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
int start_server(int port, int listen_queue, int timeout, int fd_ro, int fd_wr)
{
	struct sockaddr_in server_addr, client_addr;
	unsigned int server_sock, client_sock, addr_len;
	int fdmax,i;
 
	fd_set master;
	fd_set read_fds;
	
	int optval = 1;
	struct timeval tv;

	tv.tv_sec = timeout;
	tv.tv_usec = 0;


	//Set socket options
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(server_addr.sin_zero), '\0', 8);

	setsockopt(server_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));


	if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		fprintf(stderr, "Error binding to port\n");
		return 1;
	}


	if((listen(server_sock, listen_queue)) < 0){
		fprintf(stderr, "Error listening on port\n");
		return 1;
	}
	
	//
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	FD_SET(server_sock, &master);
	FD_SET(fd_ro, &master);
	fdmax = (server_sock>fd_wr)?server_sock:fd_wr;

	while(1)
	{
		read_fds = master;
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
		{
			fprintf(stderr, "Error select failed\n");
			return -1;
		}
		
		for(i = 0; i <= fdmax; i++)
		{
			if(FD_ISSET(i, &read_fds))
			{ 
				if(i == server_sock)
				{
					addr_len = sizeof(client_addr);
					if((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len)) != -1)
					{
						FD_SET(client_sock, &master);
						if(client_sock > fdmax) fdmax = client_sock;
						printf("New connection from %s\n", inet_ntoa(client_addr.sin_addr));
					}
				}
				else if(i==fd_ro)
				{
					int j=0;
					
					for(j = 0; j <= fdmax; j++)
					{
						if(FD_ISSET(j, &master))
						{
//							if(j != listener && j != i)
//							{
//								if(send(j, buf, nbytes, 0) == -1) perror("send() error lol!");
//								close(i);
//								FD_CLR(i, &master);
//							}
						}
					}

					
				}
				else
				{
					struct pipe_rxtx todo;
					if((todo.size = recv(i, todo.data, sizeof(BUF_SIZE), 0)) <= 0)
					{
						close(i);
						FD_CLR(i, &master);
					}
					else
					{
						todo.fd=i;
						if(send(fd_wr, &todo, sizeof(struct pipe_rxtx), 0)==-1) return -1;
					}
				}
			}
		}
	}
	close(server_sock);
	return 1;
}
