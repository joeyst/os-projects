
#include <fcntl.h> 
#include <unistd.h>
#include "image.h"

int image_open(char *filename, int truncate) {
	image_fd = open(filename, O_RDWR | O_CREAT | (O_TRUNC * truncate));
	return image_fd;
}

int image_close(void) {
	return close(image_fd);	
}
