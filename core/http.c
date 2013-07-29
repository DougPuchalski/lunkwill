#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"

/** \brief Fills the buffer with an HTTP file request answer 
 *  \returns The size of the answer to send */
int send_file(char **buffer, char *file_path, int mime_type){
	FILE *file;
	unsigned int file_size;
	
	if((file = fopen(file_path, "r")) == NULL){
		fprintf(stderr, "Could not open requested file\n");
		return -1;
	}
	
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	rewind(file);

	*buffer=(char *)malloc(BUFSIZ+file_size);

	sprintf(*buffer, "HTTP/1.1 200 OK\nContent-Length: %d\nContent-Type: %s\n\n", file_size, content_types[mime_type]);
	fread((*buffer+strlen(*buffer)), file_size, 1, file);
	fclose(file);
	
	return strlen(*buffer);
}


/** \brief Get the MIME Type
 *  \returns A mime_types value */
int get_mime(char *file_path){
	char *ptr = strrchr(file_path, '.')+1;
	if(ptr == NULL)
		return 0;
	
	if(strcmp(ptr, "txt") == 0)
		return 1;
	if(strcmp(ptr, "html") == 0 || strcmp(ptr, "htm"))
		return 2;
	if(strcmp(ptr, "js") == 0)
		return 3;
	if(strcmp(ptr, "css") == 0)
		return 4;
	if(strcmp(ptr, "png") == 0)
		return 5;
		
	return 0;
}


/** \brief Fills the buffer with an HTTP answer containing a string
 *  \returns The size of the answer to send */
int send_string(char *buffer, char *string){
	sprintf(buffer, "HTTP/1.1 200 OK\nContent-Length: %u\n\n%s", (unsigned int)strlen(string), string);
	return strlen(buffer);
}
