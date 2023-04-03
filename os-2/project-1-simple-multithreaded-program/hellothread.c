#include <pthread.h> 
#include <stdio.h>
#include <string.h>

void print_numbers(char* thread_name) {
  for (int i = 0; i < 5; i++) printf("%s: %i\n", thread_name, i);
}

int main() {
  pthread_t t1;
  pthread_t t2;
  print_numbers("hello");
}
