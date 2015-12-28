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
#include "modules_list.h"

static int alloc_clientIp(struct ip_node **head, char *ip)
{
  struct ip_node *aus = NULL;

  aus = (struct ip_node *) malloc (sizeof (struct ip_node));
  if (aus == NULL)
    return -1;

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
}

void print_ipList (struct ip_node *head)
{
  int i = 0;

  while (head != NULL) {
    printf("%d# %s\n", i++, head->client_ip);
    head = head->next;
  }
}

/*  BE CAREFUL: incomplete function, don't touch it!
 *
 *  Return values:
 *    -1) malloc fail
 *     0) element added with success
 */
/*int add_module_running(struct module_running **head, char *ip, char *name, pthread_t tid)
{

  struct module_running *aus = NULL;

  aus = (struct module_thread *) malloc (sizeof (struct module_running));
  if(aus == NULL)
    return -1;

  strcpy(aus->client_ip, ip);
  strcpy(aus->name, name);
  aus->tid = tid;


}*/
