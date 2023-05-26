
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

struct inode *iget(int inode_num) {
	// Check if inode is already in memory 
	struct inode *incore_node = find_incore(inode_num);
	if (incore_node != NULL) {
		incore_node->ref_count++;
		return incore_node;
	}

	// Check if any memory available for inode 
	struct inode *free_node = find_incore_free();
	if (free_node == NULL) {
		return NULL;
	}

	// If not in memory + memory is available, copy from disk 
	read_inode(free_node, inode_num);
	free_node->ref_count = 1;
	free_node->inode_num = inode_num;
	return free_node;
}

void iput(struct inode *in) {
	// If nothing needs the inode, then it's already been written 
	// to disk. 
	if (in->ref_count == 0) {
		return;
	}
	// Otherwise, one less thing needs it, so decrement. 
	in->ref_count--;
	// If no things need it now, then write it to disk. 
	// We don't need to explicitly free the memory/deallocate bc 
	// when finding this inode again, the data will just be overwritten 
	// in `iget`. 
	if (in->ref_count == 0) {
		write_inode(in);
	}
}

struct inode *ialloc(void) {
	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(FREE_INODE_BLOCK_NUM, block);
	int free_bit = find_free(block);
	set_free(block, free_bit, SET_TAKEN);
	bwrite(FREE_INODE_BLOCK_NUM, block);

	// Could cause error, maybe should move check for if 
	// free_bit == -1 to before `set_free`. 
	if (free_bit == -1){
		return NULL;
	}

	struct inode* incore = iget(free_bit);
	if (incore == NULL){
		return NULL;
	}
	incore->size = 0;
	incore->owner_id = 0;
	incore->permissions = 0;
	incore->flags = 0;
	for (int i = 0; i < INODE_PTR_COUNT; i++){
		incore->block_ptr[i] = 0;
	}
	incore->inode_num = free_bit;
	write_inode(incore);
	return incore;
}
