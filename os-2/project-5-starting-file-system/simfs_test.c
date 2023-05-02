
#include "image.h"
#include "ctest.h"
#include <string.h>
#include <stdlib.h>
#include "block.h"
#include "free.h"

int image_fd;

#ifdef CTEST_ENABLE
void test_image(void) {
	image_open("image_file.txt", 0);
	image_close();
}

void test_block(void) {
	image_open("image_file.txt", 0);
	
	// Writing
	unsigned char* buf_write = calloc(sizeof(unsigned char), 4096);
	memcpy(buf_write, "0123", 4);
	bwrite(0, buf_write);

	// Reading 
	unsigned char* buf_read = calloc(sizeof(unsigned char), 4096);
	bread(0, buf_read);

	CTEST_ASSERT(memcmp(buf_read, "0123", 4) == 0, "testing read/write");
	image_close();
}

void test_free(void) {
	unsigned char* block = calloc(sizeof(unsigned char), 4096);
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

}
#endif

int main() {
	#ifdef CTEST_ENABLE
	CTEST_VERBOSE(1);

	test_image();
	test_block();
	test_free();
	test_inode();

	CTEST_RESULTS();
	CTEST_EXIT();
	#endif
}