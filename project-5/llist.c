
#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void llist_insert_head(struct node **head, struct node *n) {
  n->next = *head;
  *head = n;
}

void llist_insert_tail(struct node **head, struct node *n) {
  if (*head == NULL) {
    *head = n;
    return;
  }

  struct node *tail = *head;
  while (tail->next != NULL) tail = tail->next;
  tail->next = n;
}

void llist_print(struct node *head) {
  if (head == NULL) {
    printf("[empty]\n");
    return;
  }

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
  if (*head == NULL) return NULL;

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
  /*
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

    for (int i = 0; i < 5; i++) {
      llist_delete_head(&head);
      llist_print(head);
    }
    
    llist_free(&head);
    llist_print(head);

    struct node *head10 = NULL;
    struct node *n1 = node_alloc(10);
    llist_insert_head(&head10, n1);
    llist_print(head10);
    llist_insert_head(&head10, node_alloc(20));
    llist_print(head10);
    llist_insert_tail(&head10, node_alloc(30));
    llist_print(head10);
    struct node *head11 = NULL;
    llist_insert_tail(&head11, node_alloc(40));
    llist_print(head11);
    llist_insert_tail(&head11, node_alloc(50));
    llist_print(head11);
    struct node *head12 = NULL;
    llist_free(&head12);
    llist_print(head12);
  */

  struct node *head = NULL;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "ih") == 0) {
      llist_insert_head(&head, node_alloc(atoi(argv[i+1])));
      i++;
    } 

    else if (strcmp(argv[i], "it") == 0) {
      llist_insert_tail(&head, node_alloc(atoi(argv[i+1])));
      i++;
    }

    else if (strcmp(argv[i], "dh") == 0) {
      llist_delete_head(&head);
    }

    else if (strcmp(argv[i], "f") == 0) {
      llist_free(&head);
    }

    else if (strcmp(argv[i], "p") == 0) {
      llist_print(head);
    }

  }
}
