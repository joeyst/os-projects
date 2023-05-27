
#ifndef INODE_H
#define INODE_H
#define FREE_INODE_BLOCK_NUM 1
#define INODE_PTR_COUNT 16
#define MAX_SYS_OPEN_FILES 64

int get_block_num_from_inode_num(int);
int get_block_offset_from_inode_num(int);
int get_block_offset_bytes_from_block_offset(int);
int block_offset_bytes_from_inode_num(int);

struct inode {
	unsigned int size;
	unsigned short owner_id;
	unsigned char permissions;
	unsigned char flags;
	unsigned char link_count;
	unsigned short block_ptr[INODE_PTR_COUNT];

	unsigned int ref_count;
	unsigned int inode_num;
};

struct inode *find_incore_free(void);
struct inode *find_incore(unsigned int);
void read_inode(struct inode *, int);
void write_inode(struct inode *);
struct inode *iget(int inode_num);
void iput(struct inode *);
struct inode *ialloc(void);

#endif
