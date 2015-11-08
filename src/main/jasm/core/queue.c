#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "queue.h"

void print_queue(struct queue *head)
{
  if(head==NULL) {
    perror("head is NULL");
    return;
  }

  while(head!=NULL) {
    printf("# %s\n", head->string);
    head=head->next;
  }
}

/*
 *  Aggiunge elemento alla coda.
 *  Return 0 in caso di inserimento corretto
 */
int add_queue(struct queue **head, char temp[BUFSIZ])
{

  if((*head)==NULL) { //coda vuota

    (*head)=(struct queue *)malloc(sizeof(struct queue));
    if(*head==NULL) {
      perror("malloc fail");
      return 1;
    }
    strcpy((*head)->string, temp);
    (*head)->next=NULL;

  } else {  //coda non vuota

    struct queue *aus=(*head);
    while(aus->next!=NULL)
      aus=aus->next;

    aus->next=(struct queue *)malloc(sizeof(struct queue));
    aus=aus->next;

    if(aus==NULL) {
      perror("malloc fail");
      return 1;
    }
    strcpy(aus->string, temp);
    aus->next=NULL;
  }
  return 0;
}

/*
 *  Rimuove un elemento dalla coda.
 *  Return messaggio dell'elemento eliminato
 */
char *del_queue(struct queue **head)
{
  static char temp[BUFSIZ]="null";

  if((*head)==NULL) {
    perror("head is NULL");
    return temp;
  }

  struct queue *aus=(*head);
  (*head)=(*head)->next;
  strcpy(temp, aus->string);

  free(aus);
  return temp;
}
