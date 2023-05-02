
#include <unistd.h>
#include "image.h"
#include "block.h"
#include <stdlib.h>
#include "free.h"


/*
`lseek`
https://pubs.opengroup.org/onlinepubs/009696799/functions/lseek.html
`lseek(int fildes, off_t offset, int whence)`. 
`whence` can be `SEEK_SET`, `SEEK_CUR`, `SEEK_END` => set, add, past file end. 
*/

/*
Given blank buffer, copy data present at `block_num` into blank buffer, 
returning pointer to now not-blank buffer. 
*/
unsigned char *bread(int block_num, unsigned char *block) {
	lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET);
	read(image_fd, block, BLOCK_SIZE);
	return block;
}

/*
Given not-blank buffer, copy data present in not-blank buffer into 
specified block number's location. 
*/
void bwrite(int block_num, unsigned char *block) {
	lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET);
	write(image_fd, block, BLOCK_SIZE);
}

int alloc(void) {
	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(2, block);
	int free_bit = find_free(block);
	set_free(block, free_bit, 1);
	bwrite(2, block);
	return free_bit;
}
