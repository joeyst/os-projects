
#include <stdio.h>
#include <sys/mman.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))

struct block {
  struct block *next;
  int size;     // Bytes
  int in_use;   // Boolean
};

struct block *head = NULL;  // Head of the list, empty
int padded_struct_block_size = PADDED_SIZE(sizeof(struct block));

void *myalloc(int size) {
  int padded_block_size = PADDED_SIZE(sizeof(struct block));

  // in my_malloc()
  if (head == NULL) {
    head = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    head->next = NULL;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    head->in_use = 0;
  }

  struct block *cur = head;

  while (cur != NULL) {
    if (cur->in_use == 0 && cur->size >= size) {
      cur->in_use = 1;
      return PTR_OFFSET(cur, padded_block_size);
    }
    cur = cur->next;
  }

  return NULL;
}

void print_data(void)
{
  struct block *b = head;

  if (b == NULL) {
    printf("[empty]\n");
    return;
  }

  while (b != NULL) {
    // Uncomment the following line if you want to see the pointer values
    // printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
    printf("[%d,%s]", b->size, b->in_use? "used": "free");
    if (b->next != NULL) {
        printf(" -> ");
    }

    b = b->next;
  }

  printf("\n");
}

int main() {
  /*
  void *p;

  print_data();
  p = myalloc(64);
  print_data();
  */

  void *p;

  print_data();
  p = myalloc(16);
  print_data();
  p = myalloc(16);
  printf("%p\n", p);
}
