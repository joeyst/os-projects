
#include "eventbuf.h"
#include "pthread.h"
#include "stdlib.h"
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

struct eventbuf *buf;

sem_t *sem_open_temp(const char *name, int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void* producer_script(void* arg) {
  int* id = (int*)arg;

  return NULL;
}

void* consumer_script(void* arg) {
  int* id = (int*)arg;

  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc != 5) printf("Use: # prod, # cons, # events, # outstanding");

  buf = eventbuf_create();

  int num_producers   = atoi(argv[1]);
  int num_consumers   = atoi(argv[2]);
  int num_events      = atoi(argv[3]);
  int num_outstanding = atoi(argv[4]);

  for (int i = 0; i < num_producers; i++) {
    pthread_t producer; 
    pthread_create(&producer, NULL, producer_script, (void*)&i);
  }
}
