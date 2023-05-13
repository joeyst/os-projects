
#ifndef INODE_H
#define INODE_H
#define FREE_INODE_BLOCK_NUM 1
#define INODE_PTR_COUNT 16
#define MAX_SYS_OPEN_FILES 64

struct inode {
    unsigned int size;
    unsigned short owner_id;
    unsigned char persmissions;
    unsigned char flags;
    unsigned char link_count;
    unsigned short block_ptr[INODE_PTR_COUNT];

    unsigned int ref_count;
    unsigned int inode_num;

};

int ialloc(void);

#endif
