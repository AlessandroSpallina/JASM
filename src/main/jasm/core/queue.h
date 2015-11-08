#ifndef _QUEUE_H
#define _QUEUE_H

struct queue {
  char string[BUFSIZ];
  struct queue *next;
};

extern void print_queue(struct queue *head);
extern int add_queue(struct queue **head, char temp[BUFSIZ]);
extern char *del_queue(struct queue **head);

#endif
