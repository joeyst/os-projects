
#include "eventbuf.h"
#include "pthread.h"
#include "stdlib.h"
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

struct eventbuf *buf;
sem_t *items;
sem_t *mutex;
sem_t *spaces;

int num_producers; 
int num_consumers; 
int num_events; 
int num_outstanding; 

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
  int* thread_id = (int*)arg;
  int event_id;

  // Creating the number of events for this specific producer. 
  for (int i = 0; i < num_events; i++) {
    event_id = *thread_id * 100 + i;
    // Waiting until there are spaces available and that the producer 
    // has exclusive access to the buffer. 
    sem_wait(spaces);
    sem_wait(mutex);
    printf("P%d: adding event %d\n", *thread_id, event_id);
    eventbuf_add(buf, event_id);
    // Releasing the mutex and posting that there is another item in the buffer. 
    sem_post(mutex);
    sem_post(items);
  }

  printf("P%d: exiting\n", *thread_id);
  pthread_exit(NULL);
}

void* consumer_script(void* arg) {
  int* id = (int*)arg;
  int event_id;

  // Consuming 
  while (1) {
    // Waiting until it is signaled that there is an item 
    // in the buffer. 
    sem_wait(items);
    sem_wait(mutex);

    // If the consumer is awoken and the buffer is empty, 
    // then we know to release the mutex, print, and exit. 
    if (eventbuf_empty(buf)) {
      sem_post(mutex);
      printf("C%d: exiting\n", *id);
      pthread_exit(NULL);
    }

    // Getting the event ID from the buffer and posting for 
    // the producers. 
    event_id = eventbuf_get(buf);
    printf("C%d: got event %d\n", *id, event_id);
    sem_post(mutex);
    sem_post(spaces);
  }
}

int main(int argc, char* argv[]) {
  if (argc != 5) printf("Use: # prod, # cons, # events, # outstanding\n");

  buf = eventbuf_create();

  num_producers   = atoi(argv[1]);
  num_consumers   = atoi(argv[2]);
  num_events      = atoi(argv[3]);
  num_outstanding = atoi(argv[4]);

  items = sem_open_temp("items", 0);
  mutex = sem_open_temp("mutex", 1);
  spaces = sem_open_temp("spaces", num_outstanding);

  pthread_t *producers = calloc(sizeof(pthread_t), num_producers);

  for (int i = 0; i < num_producers; i++) {
    pthread_t producer_thread;
    producers[i] = producer_thread;
    pthread_create(&producer_thread, NULL, producer_script, (void*)&i);
  }

  for (int i = 0; i < num_consumers; i++) {
    pthread_t consumer_thread; 
    pthread_create(&consumer_thread, NULL, consumer_script, (void*)&i);
  }

  for (int i = 0; i < num_producers; i++) {
    pthread_join(producers[i], NULL);
  }

  for (int i = 0; i < num_consumers; i++) {
    sem_post(items);
  }
}
