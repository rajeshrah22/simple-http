
struct node {
  struct node *next;
  struct node *prev;
  int data;
};

struct my_queue {
  struct node *head;
  struct node *tail;
};


int is_empty(struct my_queue *queuestrct);
void init_queue(struct my_queue *queuestrct);
void en_queue(struct my_queue *queuestrct, const int cfd);
int de_queue(struct my_queue *queuestrct);
void destroy_queue(struct my_queue *queuestrct);