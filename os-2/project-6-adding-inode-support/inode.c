
#include "inode.h"
#include "free.h"
#include <stdlib.h>
#include "block.h"
#include "pack.h"

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
	for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
		if (incore[i].ref_count == 0) {
			return &incore[i];
		}
	}
	return NULL;
}

struct inode *find_incore(unsigned int inode_num){
	for (int i = 0; i < MAX_SYS_OPEN_FILES;i++){
		if (incore[i].inode_num == inode_num && incore[i].ref_count != 0){
			return &incore[i];
		}
	}
	return NULL;
}

void read_inode(struct inode *in, int inode_num) {
	int block_num = get_block_num_from_inode_num(inode_num);
	int block_offset_bytes = block_offset_bytes_from_inode_num(inode_num);

	// Creating a free block of memory. 4096 bytes. 
	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	// Copying data from disk into our own temporary 4096 byte block. 
	bread(block_num, block);

	// Copying data from our temporary block into the inode struct.
	in->size = read_u32(block + block_offset_bytes);
	in->owner_id = read_u16(block + block_offset_bytes + 4);
	in->permissions = read_u8(block + block_offset_bytes + 6);
	in->flags = read_u8(block + block_offset_bytes + 7);
	in->link_count = read_u8(block + block_offset_bytes + 8);
	for (int i = 0; i < INODE_PTR_COUNT; i++) {
		in->block_ptr[i] = read_u16(block + block_offset_bytes + 9 + (i * 2));
	}
	in->inode_num = inode_num;
}

void write_inode(struct inode *in) {
	int block_num = get_block_num_from_inode_num(in->inode_num);
	int block_offset_bytes = block_offset_bytes_from_inode_num(in->inode_num);

	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(block_num, block);

	write_u32(block + block_offset_bytes, in->size);
	write_u16(block + block_offset_bytes + 4, in->owner_id);
	write_u8(block + block_offset_bytes + 6, in->permissions);
	write_u8(block + block_offset_bytes + 7, in->flags);
	write_u8(block + block_offset_bytes + 8, in->link_count);
	for (int i = 0; i < INODE_PTR_COUNT; i++) {
		write_u16(block + block_offset_bytes + 9 + (i * 2), in->block_ptr[i]);
	}
	bwrite(block_num, block);
}

int ialloc(void) {
	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(FREE_INODE_BLOCK_NUM, block);
	int free_bit = find_free(block);
	set_free(block, free_bit, SET_TAKEN);
	bwrite(FREE_INODE_BLOCK_NUM, block);
	return free_bit;
}
