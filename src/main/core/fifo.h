#ifndef _FIFO_H
#define _FIFO_H

/*
 * predisposizione alla flessibilità del tipo di messaggio
 */
 struct message {
   char string[BUFSIZ];
 };

struct command {
  struct message message;
  struct command *next;
};

extern void print_fifo(struct command *head);
extern int add(struct command **head, struct message temp);
extern struct message del(struct command **head);

#endif
