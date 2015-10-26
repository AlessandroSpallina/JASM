#ifndef _FIFO_H
#define _FIFO_H

/*
 * predisposizione alla flessibilità di messaggio
 */
 struct message {
   char string[BUFSIZ];
 };

struct command {
  struct message message;
  struct command *next;
};

#endif
