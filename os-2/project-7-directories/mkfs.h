
#ifndef MKFS_H
#define MKFS_H

#define BLOCK_COUNT 1024

unsigned char *make_directory_entry(int, char*);
void mkfs(void);
struct directory {
    struct inode *inode;
    unsigned int offset;
};
struct directory_entry {
    unsigned int inode_num;
    char name[16];
};
struct directory *directory_open(int);
int directory_get(struct directory *, struct directory_entry *);
void directory_close(struct directory *);

#endif 
