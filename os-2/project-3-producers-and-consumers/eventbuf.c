#include <stdlib.h>
#include "eventbuf.h"

struct eventbuf_node {
    int event;
    struct eventbuf_node *next;
};

struct eventbuf *eventbuf_create(void)
{
    struct eventbuf *eb = malloc(sizeof *eb);

    if (eb == NULL) return NULL;

    eb->head = eb->tail = NULL;

    return eb;
}

void eventbuf_free(struct eventbuf *eb)
{
    free(eb);
}

int eventbuf_add(struct eventbuf *eb, int event)
{
    struct eventbuf_node *node = malloc(sizeof *node);

    if (node == NULL) return -1;

    node->event = event;
    node->next = NULL;

    if (eb->head == NULL) {
        eb->head = eb->tail = node;
    } else {
        eb->tail->next = node;
        eb->tail = node;
    }

    return 0;
}

int eventbuf_get(struct eventbuf *eb)
{
    if (eb->head == NULL) return 0;

    struct eventbuf_node *node = eb->head;
    int event = node->event;

    eb->head = eb->head->next;

    free(node);

    return event;
}

int eventbuf_empty(struct eventbuf *eb)
{
    return eb->head == NULL;
}

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
