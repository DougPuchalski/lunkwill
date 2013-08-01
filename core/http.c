#include "http.h"
#include "lunkwill.h"

/** \brief Fills the buffer with an HTTP file request answer 
 *  \returns The size of the answer to send */
int send_file(char **buffer, char *file_path){
	FILE *file;
	unsigned int file_size;
	struct stat statbuf;

	if(stat(file_path, &statbuf) == -1) {
		fprintf(stderr, "Could not open requested file\n");
		return -1;
	}
	
	if(!S_ISREG(statbuf.st_mode)){
		fprintf(stderr, "Could not open requested file\n");
		return -1;
	}
	
	if((file = fopen(file_path, "r")) == NULL){
		fprintf(stderr, "Could not open requested file\n");
		return -1;
	}
	
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	rewind(file);

	*buffer=(char *)malloc(BUF_SIZE+file_size);

	sprintf(*buffer, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %u\r\nConnection: close\r\n\r\n", content_types[get_mime(file_path)], file_size );
	file_size+=strlen(*buffer);
	fread((*buffer+strlen(*buffer)), file_size, 1, file);
	fclose(file);
	
	return file_size;
}


/** \brief Get the MIME Type
 *  \returns A mime_types value */
int get_mime(char *file_path){
	char *ptr = strrchr(file_path, '.');
	if(ptr == NULL)
		return 0;
	ptr++;
	
	if(strcasecmp(ptr, "txt") == 0)
		return 1;
	if(strcasecmp(ptr, "html") == 0 || strcasecmp(ptr, "htm") == 0)
		return 2;
	if(strcasecmp(ptr, "js") == 0)
		return 3;
	if(strcasecmp(ptr, "css") == 0)
		return 4;
	if(strcasecmp(ptr, "png") == 0)
		return 5;
		
	return 0;
}


/** \brief Fills the buffer with an HTTP answer containing a string
 *  \returns The size of the answer to send */
int send_string(char **buffer, char *string){
	*buffer=malloc(strlen(string)+BUF_SIZE);	
	sprintf(*buffer, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %u\r\nConnection: close\r\n\r\n%s", (unsigned int)strlen(string), string);
	return strlen(*buffer);
}
