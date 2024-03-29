#include "ls.h"
#include "mkfs.h"
#include "stdio.h"
void ls(void)
{
    struct directory *dir;
    struct directory_entry ent;

    dir = directory_open(0);

    while (directory_get(dir, &ent) != -1) {
        printf("Offset: %d\n", dir->offset);
        printf("%d %s\n", ent.inode_num, ent.name);
    }

    directory_close(dir);
}