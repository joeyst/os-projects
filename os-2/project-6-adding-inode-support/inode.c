
#include "inode.h"
#include "free.h"
#include <stdlib.h>
#include "block.h"

int ialloc(void) {
	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(FREE_INODE_BLOCK_NUM, block);
	int free_bit = find_free(block);
	set_free(block, free_bit, SET_TAKEN);
	bwrite(FREE_INODE_BLOCK_NUM, block);
	return free_bit;
}
