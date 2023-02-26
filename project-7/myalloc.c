
#include <stdio.h>
#include <sys/mman.h>
#include <stdbool.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))

struct block {
  struct block *next;
  int size;     // Bytes
  int in_use;   // Boolean
};

bool not_in_use(struct block *cur) {
  return cur->in_use == 0;
}

bool is_big_enough(struct block *cur, int size) {
  return cur->size >= size;
}

bool is_big_enough_to_split(struct block *cur, int size) {
  return cur->size >= (size + PADDED_SIZE(sizeof(struct block)));
}

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
    // if space is big enough: 
    if (not_in_use(cur) && is_big_enough(cur, size)) {
      // vvv splitting code vvv

      // if the space is big enough to split: 
      
      // TODO 

      // ^^^ splitting code ^^^
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
