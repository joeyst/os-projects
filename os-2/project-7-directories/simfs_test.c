
#include "image.h"
#include "ctest.h"
#include <string.h>
#include <stdlib.h>
#include "block.h"
#include "free.h"
#include "inode.h"
#include "mkfs.h"
#include "ls.h"

#ifdef CTEST_ENABLE
void test_image(void) {
	image_open("image_file.txt", 0);
	image_close();
}

void test_block(void) {
	image_open("image_file.txt", 0);
	
	// Writing
	unsigned char* buf_write = calloc(sizeof(unsigned char), BLOCK_SIZE);
	memcpy(buf_write, "0123", 4);
	bwrite(0, buf_write);

	// Reading 
	unsigned char* buf_read = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(0, buf_read);

	CTEST_ASSERT(memcmp(buf_read, "0123", 4) == 0, "testing read/write");
	image_close();

	image_open("image_file.txt", 0);
	int first_free = alloc();
	CTEST_ASSERT(first_free == 0, "test getting first free block");
	int second_free = alloc();
	CTEST_ASSERT(second_free == 1, "test getting second free block");
	unsigned char* block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(2, block);
	CTEST_ASSERT(block[0] == 0b00000011, "test correct bits marked as taken");
	image_close();
}

void test_free(void) {
	unsigned char* block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	set_free(block, 7, 1);
	CTEST_ASSERT(block[0] == (1 << 7), "testing setting 7th bit to 1 when previously 0");
	set_free(block, 7, 0);
	CTEST_ASSERT(block[0] == 0, "testing setting 7th bit to 0 when previously 1");
	set_free(block, 7, 0);
	CTEST_ASSERT(block[0] == 0, "testing setting 7th bit to 0 when previously 0");
	set_free(block, 7, 1);
	set_free(block, 7, 1);
	CTEST_ASSERT(block[0] == (1 << 7), "testing setting 7th bit to 1 when previously 1");
}

void test_inode(void) {
	image_open("image_file.txt", 0);
	int first_free = ialloc()->inode_num;
	CTEST_ASSERT(first_free == 0, "test getting first free block");
	int second_free = ialloc()->inode_num;
	CTEST_ASSERT(second_free == 1, "test getting second free block");
	unsigned char* block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(2, block);
	CTEST_ASSERT(block[0] == 0b00000011, "test correct bits marked as taken");
	image_close();
}

void test_read_inode_write_inode(void) {
	image_open("image_file.txt", 0);
	
	image_close();
}

void test_mkfs(void) {
	image_open("image_file.txt", 0);
	mkfs();
	unsigned char *block = calloc(sizeof(unsigned char), BLOCK_SIZE);
	bread(0, block);
	CTEST_ASSERT(memcmp(block, "\0\0\0\0", 4) == 0, "testing zeroing in mkfs");
	int next_free_block = alloc();
	CTEST_ASSERT(next_free_block == 8, "testing next free block in newly initialized file system is 7");
	image_close();
}

#endif

int main() {
	#ifdef CTEST_ENABLE
	CTEST_VERBOSE(1);

	test_image();
	test_block();
	test_free();
	test_inode();
	test_read_inode_write_inode();
	test_mkfs();
	printf("========================================\n");
	mkfs();
	ls();

	CTEST_RESULTS();
	CTEST_EXIT();
	#endif
}