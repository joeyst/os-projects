
#include "mkfs.h"
#include "block.h"
#include <stdlib.h>
#include "image.h"
#include <unistd.h>
#include "pack.h"
#include "inode.h"
#include <string.h>

// Not sure where to put these yet-- will move later. 
#ifndef DIRECTORY_FLAG
#define DIRECTORY_FLAG 2
#endif 

#ifndef INITIAL_DIRECTORY_SIZE 
#define INITIAL_DIRECTORY_SIZE (2 * 32) 
#endif

#ifndef ROOT_DIRECTORY_INODE_NUM 
#define ROOT_DIRECTORY_INODE_NUM 0 
#endif 

#ifndef DIRECTORY_ENTRY_SIZE 
#define DIRECTORY_ENTRY_SIZE 32 
#endif  

#define DATA_BLOCK_FROM_OFFSET(offset) (offset / BLOCK_SIZE)

#define START_INDEX_OF_FILE_NAME_IN_DIRECTORY_ENTRY 2 

unsigned char *make_directory_entry(int inode_num, char* name) {
	unsigned char *directory_entry = calloc(DIRECTORY_ENTRY_SIZE, sizeof(unsigned char));
	write_u16(directory_entry, inode_num);
	// Assuming that `name` here is null-terminated 
		// Fix me plez 
	strcpy((void *)&directory_entry[START_INDEX_OF_FILE_NAME_IN_DIRECTORY_ENTRY], name);
	return directory_entry;
}


void mkfs(void) {
	for (int block_num = 0; block_num < BLOCK_COUNT; block_num++) {
		char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
		lseek(image_fd, BLOCK_SIZE * block_num, SEEK_SET);
		write(image_fd, block, BLOCK_SIZE);
	}

	for (int block_num = 0; block_num < 7; block_num++) {
		alloc();
	}

	// Creating an inode and data block for directory entries. 
	struct inode *root = ialloc();
	int directory_entry_data_block_id = alloc();

	// Initializing root's inode 
	root->flags = DIRECTORY_FLAG;
	root->size = INITIAL_DIRECTORY_SIZE;
	root->block_ptr[0] = directory_entry_data_block_id;
	root->inode_num = ROOT_DIRECTORY_INODE_NUM;

	// Allocating some memory to store our data and storing it  
	unsigned char* block = calloc(BLOCK_SIZE, sizeof(unsigned char));
	char* parent_name = calloc(3, sizeof(char));
	parent_name = "..\0";
	char* current_name = calloc(3, sizeof(char));
	current_name = ".\0";
	unsigned char* parent_entry = make_directory_entry(0, parent_name);
	unsigned char* current_entry = make_directory_entry(0, current_name);
	memcpy(block, parent_entry, DIRECTORY_ENTRY_SIZE);
	memcpy(block + DIRECTORY_ENTRY_SIZE, current_entry, DIRECTORY_ENTRY_SIZE);

	// Writing it back out to disk with 
	bwrite(directory_entry_data_block_id, block);

	// Write new directory inode out to disk 
	iput(root);
}

struct directory *directory_open(int inode_num){
	struct inode* open_directory = iget(inode_num);
	if (open_directory == NULL) {
		return NULL;
	}
	struct directory *directory_struct = malloc(sizeof(struct directory));
	directory_struct->inode = open_directory;
	directory_struct->offset = 0;
	return directory_struct;

}

int directory_get(struct directory *dir, struct directory_entry *ent){
	if(dir->offset >= dir->inode->size){
		return -1;
	}
	int data_block_index = DATA_BLOCK_FROM_OFFSET(dir->offset);
	int data_block_num = dir->inode->block_ptr[data_block_index];
	unsigned char *block = calloc(BLOCK_SIZE, sizeof(unsigned char));
	bread(data_block_num, block);
	int offset_in_block = dir->offset % BLOCK_SIZE;
	ent->inode_num = read_u16(block + offset_in_block);
	strcpy(ent->name, (char *)block + offset_in_block + 2);
	dir->offset += DIRECTORY_ENTRY_SIZE;
	return 0;
}
void directory_close(struct directory *d){
	iput(d->inode);
	free(d);
}
