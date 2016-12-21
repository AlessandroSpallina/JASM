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

/*
 *  This file cointains heap management functions of atomic element
 *  [struct module_running] defined in modules_list.h
 *  Abstract Data Type: Ordered list [client_ip] of ordered list [module_running].
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __unix__
//todo
#endif

#include "modules_list.h"

#ifdef DEBUG
//#include "logger.h" //NEVER USED
//char deblog[BUFSIZ]; //NEVER USED
#endif //DEBUG

/*
 *  Return values:
 *    NULL) clientIp not found
 *    legal pointer) clientIp found, points to clientIp element
 */
struct ip_node * find_clientIp(struct ip_node *head, char *ip)
{

        while (head != NULL) {

                if (strcmp (ip, head->client_ip) == 0)
                        return head;

                head = head->next;
        }

        return NULL;
}

static int alloc_clientIp(struct ip_node **head, char *ip)
{
        struct ip_node *aus = NULL;

        aus = (struct ip_node *) malloc (sizeof (struct ip_node));
        if (aus == NULL) {
                return -1;
        }

        strcpy(aus->client_ip, ip);
        aus->modules_list = NULL;

        aus->next = (*head);
        (*head) = aus;

        return 0;
}

/*
 *  Return values:
 *    -1) malloc fails
 *    -2) element not added because it already exists
 *     0) element added with success
 */
int add_clientIp(struct ip_node **head, char *ip)
{

        if((*head) == NULL) {
                return (alloc_clientIp(head, ip));
        }

        if (strcmp(ip, (*head)->client_ip) < 0) {
                return (alloc_clientIp(head, ip));
        }

        if (strcmp(ip, (*head)->client_ip) == 0) {
                return -2;
        } else {
                if (strcmp(ip, (*head)->client_ip) > 0) {
                        return (add_clientIp(&((*head)->next), ip));
                }
        }
        return 0;
}

/*
 *  Return values:
 *     0) client removed with success
 *    -1) client not found: error
 */
int rem_clientIp(struct ip_node **listP, char *ip)
{
        struct ip_node *currP, *prevP;

        prevP = NULL;

        for (currP = (*listP); currP != NULL; prevP = currP, currP = currP->next) {

                if (strcmp(currP->client_ip, ip) == 0) {
                        if (prevP == NULL) {
                                *listP = currP->next;
                        } else {
                                prevP->next = currP->next;
                        }
                        free(currP);
                        return 0;
                }
        }

        return -1;
}

void print_ipList (struct ip_node *head)
{
        int i = 0;

        if(head == NULL) {
                printf("0# NULL!");
        } else {
                while (head != NULL) {
                        printf("%d# %s\n", i++, head->client_ip);
                        head = head->next;
                }
        }
}

struct module_running * find_module_running(struct module_running *head, char *name)
{
        while (head != NULL) {
                if(strcmp (name, head->name) == 0)
                        return head;

                head = head->next;
        }

        return NULL;
}

static int alloc_module_running(struct module_running **head, char *name, pthread_t tid)
{
        struct module_running *aus = NULL;

        aus = (struct module_running *) malloc (sizeof (struct module_running));
        if(aus == NULL) {
                return -1;
        }

        strcpy(aus->name, name);
        aus->tid = tid;

        aus->next = (*head);
        (*head) = aus;

        return 0;
}

/*
 *  Return values:
 *    -1) malloc fail
 *    -2) module already exists
 *     0) element added with success
 */
int add_module_running(struct module_running **head, char *name, pthread_t tid)
{

        if ((*head) == NULL) {
                return (alloc_module_running(head, name, tid));
        }

        int ret = strcmp (name, (*head)->name);

        if (ret < 0) {
                return (alloc_module_running(head, name, tid));
        }

        if (ret == 0) {
                return -2;
        } else {
                return (add_module_running((&((*head)->next)), name, tid));
        }
}

/*
 *  Return values:
 *    -1) module not found
 *     0) module found and deleted with success
 */
int rem_module_running(struct module_running **head, char *name)
{
        struct module_running *toremove = NULL;

        if ((toremove = find_module_running((*head), name)) != NULL) {
                struct module_running *aus = (*head);
                while(aus->next != toremove)
                        aus = aus->next;
                aus->next = toremove->next;
                free(toremove);
                return 0;

        } else {
                return -1;
        }

}

void print_moduleList(struct module_running *head)
{
        char i = 'a';

        while(head != NULL) {
                printf("    %c) %s\n", i++, head->name);
                head = head->next;
        }
}

void print_all(struct ip_node *head)
{
        int i = 0;

        while(head != NULL) {
                struct module_running *aus = head->modules_list;

                printf("%d# %s\n", i++, head->client_ip);
                if (aus != NULL) {
                        print_moduleList(aus);
                } else {
                        printf("    a) null\n");
                }
                head = head->next;
        }
}
