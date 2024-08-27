#include <stdlib.h>
#include "my_queue.h"

struct my_queue;

void init_queue(struct my_queue *queuestrct) {
  queuestrct->head = 0;
  queuestrct->tail = 0;
}

void destroy_queue(struct my_queue *queuestrct) {
  struct node *temp = 0;
  struct node *head = queuestrct->head;
  while(head) {
    temp = head;
    head = head->prev;
    if (head)
      head->next = 0;
    free(temp);
  }
  queuestrct->head = 0;
  queuestrct->tail = 0;
}

void en_queue(struct my_queue *queuestrct, const int *cfd) {
  struct node *new = (struct node*)malloc(sizeof(struct node));
  if (!new)
    return;
  new->data = *cfd;
  new->next = queuestrct->tail;
  new->prev = 0;
  if (queuestrct->tail)
    queuestrct->tail->prev = new;
  queuestrct->tail = new;
  if (!queuestrct->head)
    queuestrct->head = queuestrct->tail;
}

int de_queue(struct my_queue *queuestrct) {
  if (!queuestrct->head)
    return -1;
  int data = queuestrct->head->data;
  struct node *temp = queuestrct->head;
  queuestrct->head = queuestrct->head->prev;
  if (queuestrct->head)
    queuestrct->head->next = 0;
  if (queuestrct->tail == temp)
    queuestrct->tail = 0;
  free(temp);

  return data;
}