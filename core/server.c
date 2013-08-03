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
	
	if(strncmp(get_request, "GET /", 5) != 0)
		goto HTTP404;

	// Send index.html
	if(get_request[5] == ' ')
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
	if(ptr == NULL)
		goto HTTP404;



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


/** \brief Thread spawner */
int start_worker(int fd_ro, int fd_wr)
{
	struct _fifo *jobs=NULL;
	struct _fifo *threads=NULL;
	struct pipe_rxtx *buffer;
	
	
	while(1)
	{
		buffer=calloc(1,sizeof(struct pipe_rxtx));

		if(read(fd_ro, buffer, sizeof(struct pipe_rxtx))<=0)
		{
			nfree(buffer);
			return 1;
		}
		fifo_push(&jobs,buffer);
		printf("Socket:%d Size:%d Data:%s\n", buffer->fd, buffer->size, buffer->data);
	}
	return 0;
}

/** \brief Server which accept clients and pass them to the worker fork */
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
					if((todo.size = recv(i, todo.data, BUF_SIZE, 0)) <= 0)
					{
						close(i);
						FD_CLR(i, &master);
					}
					else
					{
						todo.fd=i;
						if(write(fd_wr, &todo, sizeof(struct pipe_rxtx))==-1) return -1;
					}
				}
			}
		}
	}
	close(server_sock);
	return 1;
}
