#include <pthread.h> 
#include <stdio.h>
#include <string.h>

void* print_numbers(void* thread_name) {
  for (int i = 0; i < 5; i++) printf("%s: %i\n", (char*)thread_name, i);
  return NULL;
}

int main() {
  printf("Launching threads\n");

  pthread_t t1;
  pthread_t t2;

  pthread_create(&t1, NULL, print_numbers, "thread 1");
  pthread_create(&t2, NULL, print_numbers, "thread 2");

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("Threads complete!\n");
}
