
#include "llist.h"
#include <stdio.h>
#include <stdlib.h>

void llist_insert_head(struct node **head, struct node *n) {
  n->next = *head;
  *head = n;
}

int main(int argc, char *argv[])
{
  struct node head;
  head.value = 1;
  head.next = NULL;
  printf("head->value = %d\n", head.value);
  printf("head->next = %p\n", head.next);
  
  struct node head1 = {1, NULL};
  printf("head1->value = %d\n", head1.value);
  printf("head1->next = %p\n", head1.next);
  printf("head1 mem addr = %p\n", &head1);

  llist_insert_head(&head1.next, &head);
  printf("head1->value = %d\n", head1.value);
  printf("head1->next = %p\n", head1.next);
  printf("head = %p", &head);
}
