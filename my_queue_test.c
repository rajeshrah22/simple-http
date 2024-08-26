#include <stdio.h>
#include "my_queue.h"

#define NTESTS 6

struct my_queue;

// test create

int test_create() {
  struct my_queue q;

  init_queue(&q);

  if (q.head == 0 && q.tail == 0)
    return 0;
   
  return 0;
}

// test delete

int test_delete() {
  return 0;
}

// test enqueue

int test_en_queue() {
  struct my_queue q;

  init_queue(&q);
  
  int cfd = 1;

  en_queue(&q, &cfd);

  if (q.head == 0 || q.tail == 0)
    return -1;
  if (q.head->data != 1)
    return -1;
  if (q.tail->data != 1)
    return -1;
  if (q.head->next != 0)
    return -1;
  if (q.tail->next != 0)
    return -1;
  if (q.head->prev != 0)
    return -1;
  if (q.tail->prev != 0)
    return -1;
  
  return 0;
}

int test_en_queue_nonempty() {
  struct my_queue q;

  init_queue(&q);
  
  int cfd = 1;
  int cfd2 = 2;

  en_queue(&q, &cfd);
  en_queue(&q, &cfd2);

  if (q.head->data != 1)
    return -1;
  if (q.head->next != 0)
    return -1;
  if (q.head->prev->data != 2)
    return -1;
  if (q.head->prev->prev != 0)
    return -1;
  if (q.tail->data != 2)
    return -1;
  if (q.tail->prev != 0)
    return -1;
  if (q.tail->next->data != 1)
    return -1;
  if (q.tail->next->next != 0)
    return -1;

  return 0;
}

// test dequeue

int test_de_queue() {
  struct my_queue q;

  init_queue(&q);
  
  int cfd = 1;

  en_queue(&q, &cfd);

  int result = de_queue(&q);

  if (result != 1)
    return -1;
  
  if (q.head == 0 && q.tail == 0)
    return 0;
 
  return 0;
}

int test_de_queue_nonempty() {
  struct my_queue q;

  init_queue(&q);
  
  int cfd = 1;
  int cfd2 = 2;

  en_queue(&q, &cfd);
  en_queue(&q, &cfd2);

  int result = de_queue(&q);

  if (result != 1)
    return -1;

  if (q.head == 0 || q.tail == 0)
    return -1;
  if (q.head->data != 2)
    return -1;
  if (q.tail->data != 2)
    return -1;
  if (q.head->next != 0)
    return -1;
  if (q.tail->next != 0)
    return -1;
  if (q.head->prev != 0)
    return -1;
  if (q.tail->prev != 0)
    return -1;
  

  return 0;
}

int main() {
  
  int (*fun_list[NTESTS]) () = {
    test_create,
    test_delete,
    test_en_queue,
    test_de_queue,
    test_en_queue_nonempty,
    test_de_queue_nonempty
  };


  printf("\nRunning Tests ... \n\n");
  for (int i = 0; i < NTESTS; i++) {
    int result = fun_list[i]();
    if (result != -1) {
      printf("Test %d PASSED \n", i);
    } else {
      printf("Test %d FAILED \n", i);
    }
  }
}