
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

int main(int argc, char *argv[])
{
  struct node head1 = {1, NULL};
  struct node head2 = {2, NULL};
  struct node head3 = {3, NULL};
  struct node head4 = {4, NULL};
  struct node head5 = {5, NULL};

  struct node *head = &head1;
  llist_insert_tail(&head, &head2);
  llist_insert_tail(&head, &head3);
  llist_insert_tail(&head, &head4);
  llist_insert_tail(&head, &head5);
  llist_print(head);
}
