
#include "mkfs.h"
#include "dirbasename.h"
#include "block.h"
#include <stdlib.h>
#include "image.h"
#include <unistd.h>
#include "pack.h"
#include "inode.h"
#include <string.h>
#include <stdio.h>

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

// Gets what block number a given offset is in.
#define DATA_BLOCK_FROM_OFFSET(offset) (offset / BLOCK_SIZE)

// Gets where to start within a block, given an offset. 
#define DATA_BLOCK_OFFSET(offset) (offset % BLOCK_SIZE)

#define START_INDEX_OF_FILE_NAME_IN_DIRECTORY_ENTRY 2 

#define MAX_PATH_LENGTH 64

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
	root->ref_count = 1;

	// Allocating some memory to store our data and storing it  
	unsigned char* block = calloc(BLOCK_SIZE, sizeof(unsigned char));
	write_u16(block, ROOT_DIRECTORY_INODE_NUM + 1);
	strcpy((char *)&block[START_INDEX_OF_FILE_NAME_IN_DIRECTORY_ENTRY], "..");
	write_u16(block + DIRECTORY_ENTRY_SIZE, ROOT_DIRECTORY_INODE_NUM);
	strcpy((char *)&block[DIRECTORY_ENTRY_SIZE + START_INDEX_OF_FILE_NAME_IN_DIRECTORY_ENTRY], ".");

	// Writing it back out to disk with 
	bwrite(directory_entry_data_block_id, block);

	// Write new directory inode out to disk 
	write_inode(root);
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

struct inode *namei(char *path){
	if (strcmp(path, "/") == 0) {
		return iget(ROOT_DIRECTORY_INODE_NUM);
	}
	else 
		return NULL;
}
int directory_make(char *path){
	// Getting directory name and new entry name.  
	char *dirname = calloc(MAX_PATH_LENGTH, sizeof(char));
	char *basename = calloc(MAX_PATH_LENGTH, sizeof(char));
	get_dirname(path, dirname); // Step 1 
	get_basename(path, basename); // Step 2 

	// Getting the inode of the parent directory. // Step 3 
	struct inode *parent = namei(dirname);

	// Allocating a new inode for our directory. 
	struct inode *new_dir = ialloc(); // Step 4
	// Allocating a new data block for our directory (well, the directory's inode). 
	// The data block will hold directory entries. 
	// We're setting our first block pointer to point to this data block, 
	// which we'll write to in step 8. 
	int alloced_block = alloc();
	if (alloced_block == -1) {
		return -1;
	}
	new_dir->block_ptr[0] = alloced_block; // Step 5
	// Creating a data block to write our entries to. // Step 6
	unsigned char *block = calloc(BLOCK_SIZE, sizeof(unsigned char));	
	// Writing our entries to the block. 
	write_u16(block, parent->inode_num);	
	strcpy((char *)&block[START_INDEX_OF_FILE_NAME_IN_DIRECTORY_ENTRY], "..");	
	write_u16(block + DIRECTORY_ENTRY_SIZE, new_dir->inode_num);	
	strcpy((char *)&block[DIRECTORY_ENTRY_SIZE + START_INDEX_OF_FILE_NAME_IN_DIRECTORY_ENTRY], "."); 
	
	// Initializing our new directory's member variables. 
	new_dir->size = INITIAL_DIRECTORY_SIZE; // Step 7
	// Writing our block to disk. (Our first block pointer points to this block.) 
	bwrite(new_dir->block_ptr[0], block); // Step 8

	// Getting the block number we'll be writing a directory to 
	int parent_data_block_num = parent->block_ptr[DATA_BLOCK_FROM_OFFSET(parent->size)]; // Step 9 
	// Allocating memory for our block and reading it into memory 
	unsigned char *parent_block = calloc(BLOCK_SIZE, sizeof(unsigned char)); // Step 10 
	bread(parent_data_block_num, parent_block);
	// Creating a directory entry for our new directory.
	int directory_entry_offset = DATA_BLOCK_OFFSET(parent->size);
	// Writing our new directory as an entry in the parent directory. 
	write_u16(parent_block + directory_entry_offset, new_dir->inode_num); // Step 10 cont. 
	strcpy((char *)&parent_block[directory_entry_offset + START_INDEX_OF_FILE_NAME_IN_DIRECTORY_ENTRY], basename);
	// Writing the parent's directory entry block back to disk now that the new entry is written. 
	bwrite(parent_data_block_num, parent_block); // Step 11
	// Updating the inode's size now that the new entry is written. 
	parent->size += DIRECTORY_ENTRY_SIZE; // Step 12
	// Writing the new directory's inode to disk. 
	iput(new_dir); // Step 13
	// Writing the parent's inode to disk. 
	iput(parent); // Step 14
	// Returning `0` to indicate success. 
	return 0;
}