#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "fifo.h"

void print_fifo(struct command *head)
{
  if(head==NULL) {
    perror("head is NULL");
    return;
  }

  while(head!=NULL) {
    printf("# %s\n", head->message.string);
    head=head->next;
  }
}


/*
 *  Aggiunge elemento alla coda.
 *  Return 0 in caso di inserimento corretto
 */
int add(struct command **head, struct message temp)
{

  if((*head)==NULL) { //coda vuota

    (*head)=(struct command *)malloc(sizeof(struct command));
    if(*head==NULL) {
      perror("malloc fail");
      return 1;
    }
    (*head)->message=temp;
    (*head)->next=NULL;

  } else {  //coda non vuota

    struct command *aus=(*head);
    while(aus->next!=NULL)
      aus=aus->next;

    aus->next=(struct command *)malloc(sizeof(struct command));
    aus=aus->next;

    if(aus==NULL) {
      perror("malloc fail");
      return 1;
    }
    aus->message=temp;
    aus->next=NULL;

  }

  return 0;
}

/*
 *  Rimuove un elemento dalla coda.
 *  Return messaggio dell'elemento eliminato
 */
struct message del(struct command **head)
{
  struct message temp;

  if((*head)==NULL) {
    perror("head is NULL");
    return temp;
  }

  struct command *aus=(*head);
  (*head)=(*head)->next;
  temp=aus->message;
  free(aus);
  return temp;

}
