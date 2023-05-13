
#include "inode.h"
#include "free.h"
#include <stdlib.h>
#include "block.h"

#define BLOCK_SIZE 4096
#define INODE_SIZE 64
#define INODE_FIRST_BLOCK 3

#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)

int get_block_num_from_inode_num(int inode_num) {
	return inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
}

int get_block_offset_from_inode_num(int inode_num) {
	return inode_num % INODES_PER_BLOCK;
}

int get_block_offset_bytes_from_block_offset(int block_offset) {
	return block_offset * INODE_SIZE;
}

int block_offset_bytes_from_inode_num(int inode_num) {
	int block_offset = get_block_offset_from_inode_num(inode_num);
	return get_block_offset_bytes_from_block_offset(block_offset);
}

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

struct inode *find_incore_free(void) {
	
}

int ialloc(void) {
	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(FREE_INODE_BLOCK_NUM, block);
	int free_bit = find_free(block);
	set_free(block, free_bit, SET_TAKEN);
	bwrite(FREE_INODE_BLOCK_NUM, block);
	return free_bit;
}
