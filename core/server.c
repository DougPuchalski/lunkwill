#include "server.h"

pthread_mutex_t lock_send=PTHREAD_MUTEX_INITIALIZER;
int send_fd=0;

pthread_mutex_t lock_count=PTHREAD_MUTEX_INITIALIZER;
int thread_count=0;

struct _fifo *jobs=NULL;


/** \brief Parses a GET request 
 *  \param The GET request string to parse
 *  \returns - A pointer to a request request struct which has to be freed by the caller
 * 			 - NULL if the request is not valid
 */  
request parse_request(char *get_request)
{
	request req;
	
	// Invalid request
	if(strncmp(get_request, "GET /", 5) != 0)
		goto HTTP404;

	// Send index.html
	if(get_request[5] == ' ')
	{
		req.special_file = INDEX_HTML;
		return req;
	}

	/// \brief Send logo or favicon, see server.h for list
	if(strncmp(get_request+5, "logo.png ", 9) == 0)
	{
		req.special_file = LOGO_PNG;
		return req;
	}
	
	if(strncmp(get_request+5, "favicon.ico ", 11) == 0)
	{
		req.special_file = FAVICON_ICO;
		return req;
	}
	

	// Set special_file to 0 by default
	req.special_file = NON_SPECIAL;

	// Check session id
	char *ptr = strstr(get_request+5, "/");
	
	// Invalid session id
	if((ptr - (get_request+5)) != 20)
		goto HTTP404;

	// Read session id
	strncpy(req.session_id, get_request+5, 20);

	
	// Check project id
	ptr = strstr(get_request+26, "/");
	
	// Invalid project id
	if((ptr - (get_request+26)) != 4)
		goto HTTP404;

	// Read project id
	strncpy(req.project_id, get_request+26, 4);

	
	// Check module id
	ptr = strstr(get_request+31, "/");
	
	// Invalid module id
	if(ptr - (get_request+31) != 2)
		goto HTTP404;

	strncpy(req.module_id, get_request+31, 2);

	// Find end of module_request
	ptr = strstr(get_request+34, " ");
	if(ptr == NULL)
		goto HTTP404;



	if(ptr - (get_request+34) < BUFSIZ-1)
		strncpy(req.module_request, get_request+34, ptr - (get_request+34));
	else
		strncpy(req.module_request, get_request+34, BUFSIZ-1);

	return req;

	// Returns NULL
	HTTP404:
		req.special_file = ERROR_404;
		return req;
}


void *workerthread()
{
	dbgprintf("New pthread startet%s\n","");

	while(1)
	{
		struct pipe_rxtx *buffer=NULL;

		pthread_mutex_lock( &lock_count );
			buffer=fifo_pop(&jobs);
		pthread_mutex_unlock( &lock_count );

		if(buffer==NULL)
		{
			dbgprintf("No more work todo%s\n","");
			goto IQUITTODAY;
		}
		request parsed_request=parse_request(buffer->data);
		nfree(buffer->data);

		switch(parsed_request.special_file)
		{
			case NON_SPECIAL:
				//~ login_request(&parsed_request);
				break;
			case INDEX_HTML:
				dbgprintf("Send %s\n", "index.html");
				buffer->size=send_file(&buffer->data, "www/index.html");
				goto PIPE;
				break;
			case LOGO_PNG:
				dbgprintf("Send %s\n", "logo.png");
				buffer->size=send_file(&buffer->data, "www/logo.png");
				goto PIPE;
				break;
			case FAVICON_ICO:
				dbgprintf("Send %s\n", "favicon.ico");
				buffer->size=send_file(&buffer->data, "www/favicon.ico");
				goto PIPE;
				break;
			default:
				dbgprintf("Send unknown:%d\n", parsed_request.special_file);
				break;
		}
				
		
		dbgprintf("Send %s\n", "HTTP 404");
		buffer->size=strlen(HTTP_404);
		buffer->data=malloc(buffer->size);
		strncpy(buffer->data, HTTP_404, buffer->size);
		
		PIPE:		
			pthread_mutex_lock( &lock_send );
				dbgprintf("Send %d bytes through pipe %d\n", buffer->size, send_fd);
				if(write(send_fd, buffer, sizeof(struct pipe_rxtx))==-1)
				{
					nfree(buffer->data);
					nfree(buffer);
					dbgprintf("Pipe %d is broken\n", send_fd);
					pthread_mutex_unlock( &lock_send );
					goto IQUITTODAY;
				}
				if(write(send_fd, buffer->data, buffer->size)==-1)
				{
					nfree(buffer->data);
					nfree(buffer);
					dbgprintf("Pipe %d is broken\n", send_fd);
					pthread_mutex_unlock( &lock_send );
					goto IQUITTODAY;
				}
			pthread_mutex_unlock( &lock_send );
		
		nfree(buffer->data);
		nfree(buffer);

	}

	IQUITTODAY:
	pthread_mutex_lock( &lock_count );
		thread_count--;
	pthread_mutex_unlock( &lock_count );
	pthread_exit(NULL);
}


/** \brief Thread spawner */
int start_worker(int max_num_threads, int fd_ro, int fd_wr)
{
	pthread_t thread;
	send_fd=fd_wr;
		
	while(1)
	{
		struct pipe_rxtx *buffer=calloc(1,sizeof(struct pipe_rxtx));

		if(read(fd_ro, buffer, sizeof(struct pipe_rxtx))<=0)
		{
			nfree(buffer);
			return 1;
		}
		
		buffer->data=calloc(1,buffer->size+2);
		if(read(fd_ro, buffer->data, buffer->size)<=0)
		{
			nfree(buffer->data);
			nfree(buffer);
			return 1;
		}

		dbgprintf("Socket:%d Size:%d\n", buffer->fd, buffer->size);		
		
		pthread_mutex_lock( &lock_count );
			fifo_push(&jobs,buffer);

			if(thread_count<max_num_threads)
			{
				int i;
				thread_count++;
				dbgprintf("New Thread -> Total active:%d\n", thread_count);		
				if((i=pthread_create( &thread, NULL, workerthread, NULL))!=0)
				{
					dbgprintf("Failed to create Thread:%d %s\n", i, strerror(errno));		
					thread_count--;					
				}
				else
				{
					pthread_detach(thread);
				}
			}
		pthread_mutex_unlock( &lock_count );
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
		dbgprintf("Error binding to port %d\n", port);
		return 1;
	}


	if((listen(server_sock, listen_queue)) < 0){
		dbgprintf("Error listening on port %d\n",port);
		return 1;
	}
	
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
			dbgprintf("Error select failed with errno: %d\n", errno);
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
						dbgprintf("New connection from %s\n", inet_ntoa(client_addr.sin_addr));
					}
				}
				else if(i==fd_ro)
				{
					struct pipe_rxtx buffer;
					if(read(fd_ro, &buffer, sizeof(struct pipe_rxtx))<=0)
					{
						return 1;
					}

					dbgprintf("Read %d bytes from pipe\n", buffer.size);

					buffer.data=malloc(buffer.size+2);
					if(read(fd_ro, buffer.data, buffer.size)<=0)
					{
						nfree(buffer.data);
						dbgprintf("Pipe %d is broken\n", fd_ro);
						return 1;
					}
					
					dbgprintf("Forward pipe data to socket %d\n", buffer.fd);

					if((send(buffer.fd, buffer.data, buffer.size, 0)) <= 0)
					{
						close(buffer.fd);
					}

					close(buffer.fd);
					nfree(buffer.data);
				}
				else
				{
					struct pipe_rxtx todo;
					todo.data=calloc(1, BUF_SIZE);
					if((todo.size = recv(i, todo.data, BUF_SIZE, 0)) <= 0)
					{
						dbgprintf("Client %d closed connection\n",i);
						close(i);
					}
					else
					{
						todo.fd=i;
						if(write(fd_wr, &todo, sizeof(struct pipe_rxtx))==-1)
						{
							nfree(todo.data);
							dbgprintf("Pipe %d is broken\n", fd_wr);
							return -1;
						}
						if(write(fd_wr, todo.data, todo.size)==-1)
						{
							dbgprintf("Pipe %d is broken\n", fd_wr);
							nfree(todo.data);
							return -1;
						}
					}
					FD_CLR(i, &master);
					nfree(todo.data);
				}
			}
		}
	}
	close(server_sock);
	return 1;
}
