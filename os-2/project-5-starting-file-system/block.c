
#include <unistd.h>
#include "image.h"
#include "block.h"

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
	lseek(image_fd, block_num * 4096, SEEK_SET);
	read(image_fd, block, 4096);
	return block;
}

/*
Given not-blank buffer, copy data present in not-blank buffer into 
specified block number's location. 
*/
void bwrite(int block_num, unsigned char *block) {
	lseek(image_fd, block_num * 4096, SEEK_SET);
	write(image_fd, block, 4096);
	return block;
}
