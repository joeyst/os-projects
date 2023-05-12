
#include "mkfs.h"
#include "block.h"
#include <stdlib.h>
#include "image.h"
#include <unistd.h>

void mkfs(void) {
	for (int block_num = 0; block_num < BLOCK_COUNT; block_num++) {
		char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
		lseek(image_fd, BLOCK_SIZE * block_num, SEEK_SET);
		write(image_fd, block, BLOCK_SIZE);
	}

	for (int block_num = 0; block_num < 7; block_num++) {
		alloc();
	}
}
