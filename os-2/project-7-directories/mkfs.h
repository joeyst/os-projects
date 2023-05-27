
#ifndef MKFS_H
#define MKFS_H

#define BLOCK_COUNT 1024

unsigned char *make_directory_entry(int, char*);
void mkfs(void);
struct directory {
    struct inode *inode;
    unsigned int offset;
};
struct directory *directory_open(int);

#endif 
