/****************************************************************************
* Copyright © 2015 Alessandro Spallina
* email: alessandrospallina1@gmail.com
* github: https://github.com/AlessandroSpallina
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "modules.h" //running_module definition
#include "queue.h" //main definitions
#include "miscellaneous.h" //misc funct

void print_queue(struct queue *head)
{
        if(head==NULL) {
                perror("[DEV] head is NULL!");
                return;
        }

        while(head!=NULL) {
                printf("# %s\n", head->string);
                head=head->next;
        }
}

/*
 *  Adds an element to the queue
 *  returns 0 if done successfully
 */
int add_queue(struct queue **head, struct running_module *temp)
{

        if((*head)==NULL) { //empty queue

                (*head)=(struct queue *)malloc(sizeof(struct queue));
                if(*head==NULL) {
                        log_error("[DEV] memory-allocation failed! {malloc()}");
                        return 1;
                }
                (*head)->info=temp;
                (*head)->next=NULL;

        } else { //not empty queue

                struct queue *aus=(*head);
                while(aus->next!=NULL)
                        aus=aus->next;

                aus->next=(struct queue *)malloc(sizeof(struct queue));
                aus=aus->next;

                if(aus==NULL) {
                        log_error("malloc fail");
                        return 1;
                }
                aus->info=temp;
                aus->next=NULL;
        }
        return 0;
}

/*
 *  deletes queue element
 *  Returns msg from deleted element
 */
struct running_module *del_queue(struct queue **head)
{
        static struct running_module *temp;
        /*{
           .head=NULL,
           .fd=-1,
           .sec=-1
           };*/
        temp->head=NULL;
        temp->fd=-1;
        temp->sec=-1;

        if((*head)==NULL) {
                log_error("[DEV] head is NULL!");
                return temp;
        }

        struct queue *aus=(*head);
        (*head)=(*head)->next;
        temp = aus->info;

        free(aus);

        return temp;
}
