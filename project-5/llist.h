
#ifndef LLIST_H
#define LLIST_H

struct node {
    int value;
    struct node *next;
};

void llist_insert_head(struct node **head, struct node *n); // done 
struct node *llist_delete_head(struct node **head); // done 
void llist_insert_tail(struct node **head, struct node *n); // done 
void llist_print(struct node *head); // done 
void llist_free(struct node **head); // done 

struct node *node_alloc(int value); // done 
void node_free(struct node *n); // done 

#endif
