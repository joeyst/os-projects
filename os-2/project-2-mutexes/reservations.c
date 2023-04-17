#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// These will be initialized in main() from the command line.
int seat_count;
int broker_count;
int *seat_taken;  // Array of seats
int transaction_count;

int seat_taken_count = 0;

// Initializing our mutex 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int reserve_seat(int n)
{
    // Attempt to reserve seat number n
    //
    // If the seat is already taken, return -1
    // Otherwise mark the seat as taken and return 0
    //
    // This function should also increment seat_taken_count if the seat
    // wasn't already taken.
    
    int return_value;
    // Locking our mutex bc `seat_taken` is a global that 
    // is modified in other functions. Hence this is a critical section. 
    pthread_mutex_lock(&mutex);
    if (seat_taken[n] == 1) {
        pthread_mutex_unlock(&mutex);
        return_value = -1;
    } else {
        seat_taken[n] = 1;
        seat_taken_count++;
        pthread_mutex_unlock(&mutex);
        return_value = 0;
    }
    return return_value; 
}

int free_seat(int n)
{
    // Attempt to free (unreserve) seat number n
    //
    // If the seat is already free, return -1
    // Otherwise mark the seat as free and return 0
    //
    // This function should also decrement seat_taken_count if the seat
    // wasn't already free.

    int return_value;
    pthread_mutex_lock(&mutex);
    if (seat_taken[n] == 0) {
        return_value = -1;
    } else {
        seat_taken[n] = 0;
        seat_taken_count--;
        return_value = 0;
    }
    pthread_mutex_unlock(&mutex);
    return return_value; 
}

int is_free(int n) {
    // Returns true if the given seat is available.

    pthread_mutex_lock(&mutex);
    // Getting a 1 value if seat is taken, else 0. 
    int return_val = seat_taken[n] == 0;
    pthread_mutex_unlock(&mutex);
    return return_val; 
}

int verify_seat_count(void) {
    // This function counts all the taken seats in the seat_taken[]
    // array.
    //
    // It then compares the count with the seat_count global variable.
    //
    // It returns true if they are the same, false otherwise
    //
    // You MAY modify this function, but the intended functionality must
    // still work properly.

    int count = 0;

    // Locking before the for loop bc each seat taken could be 
    // modified by a different thread while we're iterating thru 
    // each seat. 
    pthread_mutex_lock(&mutex);
    // Count all the taken seats
    for (int i = 0; i < seat_count; i++)
        if (seat_taken[i])
            count++;

    // Storing whether the count is correct before unlocking bc 
    // we need to return after unlocking yet also make sure we 
    // get the correct `seat_taken_count` value. 
    int return_value = count == seat_taken_count;
    pthread_mutex_unlock(&mutex);

    // Return true if it's the same as seat_taken_count
    return return_value;
}

// ------------------- DO NOT MODIFY PAST THIS LINE -------------------

void *seat_broker(void *arg)
{
    int *id = arg;

    for (int i = 0; i < transaction_count; i++) {
        int seat = rand() % seat_count;
        if (rand() & 1) {
            // buy a random seat
            reserve_seat(seat);

        } else {
            // sell a random seat
            free_seat(seat);
        }

        if (!verify_seat_count()) {
            printf("Broker %d: the seat count seems to be off! " \
                   "I quit!\n", *id);
            return NULL;
        }
    }

    printf("Broker %d: That all seemed to work very well.\n", *id);

    return NULL;
}

int main(int argc, char *argv[])
{
    // Parse command line
    if (argc != 4) {
        fprintf(stderr, "usage: reservations seat_count broker_count xaction_count\n");
        exit(1);
    }

    seat_count = atoi(argv[1]);
    broker_count = atoi(argv[2]);
    transaction_count = atoi(argv[3]);

    // Allocate the seat-taken array
    seat_taken = calloc(seat_count, sizeof *seat_taken);

    // Allocate thread handle array for all brokers
    pthread_t *thread = calloc(broker_count, sizeof *thread);

    // Allocate thread ID array for all brokers
    int *thread_id = calloc(broker_count, sizeof *thread_id);

    srand(time(NULL) + getpid());
    
    // Launch all brokers
    for (int i = 0; i < broker_count; i++) {
        thread_id[i] = i;
        pthread_create(thread + i, NULL, seat_broker, thread_id + i);
    }

    // Wait for all brokers to complete
    for (int i = 0; i < broker_count; i++)
        pthread_join(thread[i], NULL);
}

