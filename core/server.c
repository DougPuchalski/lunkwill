#include "lunkwill.h"

pthread_mutex_t lock_send=PTHREAD_MUTEX_INITIALIZER;
int send_fd=0;

pthread_mutex_t lock_count=PTHREAD_MUTEX_INITIALIZER;
int thread_count=0;

struct _fifo *jobs=NULL;

int strnmatch(char *a, char *b, int n)
{
	int i=0,j=0;

	while(!i&&j<n)
	{
		i=1;

		int x;
		for(x=0;x<strlen(b)&&i;x++)
		{
			i=!!(a[j]^b[x]);
		}
		j++;
	}
	return i;
}

int join_to_int(char *data, char *encoding, int bits, int n)
{
	int i=0,x;
	for(x=0;x<n;x++)
	{
		int y=0;

		while(1) 
		{
			if(encoding[y]==0)return -1;

			if(data[x]==encoding[y])
			{
				break;
			}
			y++;
		}

		i&=~(((int)(0xFF))<<(bits*x));
		i|=((int)(y))<<(bits*x);
	}
	i&=~(((int)(0))<<(bits*n));
	return i;
}


/** \brief Parses a GET request 
 *  \param The GET request string to parse
 *  \returns - A pointer to a request request struct which has to be freed by the caller
 * 			 - NULL if the request is not valid
 */  
request parse_request(char *get_request)
{
	request req;
	
	// Invalid request
	if(strbegin(get_request, "GET /") != 0)
	{
		dbgprintf("Invalid Request %s\n", get_request);
		goto HTTP451;
	}
	
	get_request+=5;
	
	// Send index.html
	if(strbegin(get_request," ") == 0)
	{
		dbgprintf("Request for %s\n", "index");
		req.special_file = INDEX_HTML;
		return req;
	}

	//Shared link
	if(get_request[0]=='?')
	{
		dbgprintf("Request for %s\n", "Shared Link");
		req.special_file = LINK_RESOLVER;
		return req;
	}

	/// \brief Send logo or favicon, see server.h for list
	if(strbegin(get_request, "logo.png ") == 0)
	{
		dbgprintf("Request for %s\n", "logo");
		req.special_file = LOGO_PNG;
		return req;
	}
	
	if(strbegin(get_request, "favicon.ico ") == 0)
	{
		dbgprintf("Request for %s\n", "favicon");
		req.special_file = FAVICON_ICO;
		return req;
	}
	
	// Set special_file to 0 by default
	req.special_file = NON_SPECIAL;

	if(strnmatch((get_request), url_chars, 20)!=0)
	{
		dbgprintf("No Match%s","\n");
		goto HTTP451;
	}
	
	dbgprintf("Matched login%s","\n");

	// Read session id
	strncpy(req.session_id, get_request, 20);

	req.user=join_to_int(get_request, url_chars, 6, 5);
	get_request+=5;
	req.group=join_to_int(get_request, url_chars, 6, 5);
	get_request+=5;
	req.session1=join_to_int(get_request, url_chars, 6, 5);
	get_request+=5;
	req.session2=join_to_int(get_request, url_chars, 6, 5);
	get_request+=6;

	dbgprintf("UserId: %d\n", req.user);
	dbgprintf("Group: %d\n", req.group);
	dbgprintf("Session1: %d\n", req.session1);
	dbgprintf("Session2: %d\n", req.session2);
	
		
	// Check project id
	if(strnmatch((get_request), url_chars, 4)!=0)
	{
		dbgprintf("No Project ID%s","\n");
		req.project=0;
		req.module=0;
		return req;
	}

	// Read project id
	strncpy(req.project_id, get_request, 4);
	req.project=join_to_int(get_request, url_chars, 6, 4);
	get_request+=5;

	
	// Check module id
	if(strnmatch((get_request), url_chars, 2)!=0)
	{
		dbgprintf("No Module ID%s","\n");
		req.module=0;
		return req;
	}

	strncpy(req.module_id, get_request, 2);
	req.module=join_to_int(get_request, url_chars, 6, 4);
	get_request+=3;

	// Find end of module_request
	char *ptr = strstr(get_request, " ");
	if(ptr == NULL)	goto HTTP451;

	if(ptr - (get_request) < BUFSIZ-1)
	{
		strncpy(req.module_request, get_request, ptr - (get_request));
	}
	else
	{
		goto HTTP451;
	}

	return req;

	// Returns NULL
	HTTP451:
		req.special_file = ERROR_451;
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
			dbgprintf("No more work to do%s\n","");
			goto IQUITTODAY;
		}
		request parsed_request=parse_request(buffer->data);
		nfree(buffer->data);

		switch(parsed_request.special_file)
		{
			case NON_SPECIAL:
				switch(modules[0].func(modules[0].data,&parsed_request))
				{
					case 0: //uid ok
						buffer->size=send_string(&buffer->data,"200 OK");
					break;
					case 1: //ask for login
						buffer->size=send_string(&buffer->data,"Please Login");
						break;
					case 2: //server error
						goto ERROR_500;
					default:
						goto ERROR_451;
					break;
				}
				goto PIPE;
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

		ERROR_451:
			dbgprintf("Send %s\n", "HTTP 451");
			buffer->size=strlen(HTTP_451);
			buffer->data=malloc(buffer->size);
			strncpy(buffer->data, HTTP_451, buffer->size);
			goto PIPE;
		
		ERROR_500:
			dbgprintf("Send %s\n", "HTTP 500");
			buffer->size=strlen(HTTP_500);
			buffer->data=malloc(buffer->size);
			strncpy(buffer->data, HTTP_500, buffer->size);
			goto PIPE;

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
