
#include "inode.h"
#include "free.h"
#include <stdlib.h>
#include "block.h"

#define BLOCK_SIZE 4096
#define INODE_SIZE 64
#define INODE_FIRST_BLOCK 3

#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)

int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;

int block_offset = inode_num % INODES_PER_BLOCK;

int block_offset_bytes = block_offset * INODE_SIZE;

int flags = read_u8(block + block_offset_bytes + 7);

int ialloc(void) {
	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(FREE_INODE_BLOCK_NUM, block);
	int free_bit = find_free(block);
	set_free(block, free_bit, SET_TAKEN);
	bwrite(FREE_INODE_BLOCK_NUM, block);
	return free_bit;
}
