
#include "llist.h"
#include <stdio.h>
#include <stdlib.h>

void llist_insert_head(struct node **head, struct node *n) {
  n->next = *head;
  *head = n;
}

void llist_insert_tail(struct node **head, struct node *n) {
  struct node *tail = *head;
  while (tail->next != NULL) tail = tail->next;
  tail->next = n;
}

void llist_print(struct node *head) {
  struct node *n = head;
  while (n->next != NULL) {
    printf("%d -> ", n->value);
    n = n->next;
  }
  printf("%d\n", n->value);
}

struct node *node_alloc(int value) {
  struct node *n = malloc(sizeof(struct node));
  n->value = value;
  n->next = NULL;
  return n;
}

struct node* llist_delete_head(struct node **head) {
  struct node *new_head = *head;
  new_head = new_head->next;
  node_free(*head);
  *head = new_head;
  return new_head;
}

void node_free(struct node *n) {
  free(n);
}

void llist_free(struct node **head) {
  struct node *curr_node = *head;
  while (curr_node != NULL) {
    struct node *next_node = curr_node->next;
    node_free(curr_node);
    curr_node = next_node;
  }
  *head = NULL;
}

int main(int argc, char *argv[])
{
  struct node *n = node_alloc(1);
  printf("Alloc'd: %d\n", n->value);
  node_free(n);
  printf("Freed: %d\n", n->value);
  
  struct node *head1 = node_alloc(1);
  struct node *head2 = node_alloc(2);
  struct node *head3 = node_alloc(3);
  struct node *head4 = node_alloc(4);
  struct node *head5 = node_alloc(5);

  struct node *head = head1;
  llist_insert_tail(&head, head2);
  llist_insert_tail(&head, head3);
  llist_insert_tail(&head, head4);
  llist_insert_tail(&head, head5);
  llist_print(head);

  for (int i = 0; i < 4; i++) {
    llist_delete_head(&head);
    llist_print(head);
  }
  
  // llist_free(&head);
  // llist_print(head);
}
