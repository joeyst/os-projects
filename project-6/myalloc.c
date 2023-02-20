
void *heap = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);

void *myalloc(int size) {

}

struct block {
  struct block *next;
  int size;     // Bytes
  int in_use;   // Boolean
}
