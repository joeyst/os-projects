
#include "inode.h"
#include "free.h"
#include <stdlib.h>
#include "block.h"

int ialloc(void) {
	unsigned char *block = calloc(sizeof(unsigned char), 4096);
	bread(1, block);
	int free_bit = find_free(block);
	set_free(block, free_bit, 1);
	bwrite(1, block);
	return free_bit;
}
