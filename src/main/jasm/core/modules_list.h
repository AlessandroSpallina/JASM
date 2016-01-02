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
#ifndef _MODULES_LIST_H
#define _MODULES_LIST_H

/*
 *  These structs are atomic elements of an ordered list allocated in heap.
 *  The list is ordered by client_ip, every ip_node have a pointer to an ordered
 *  list of module_running.
 *  This help us to maintain and allow only one instance of any module for
 *  client_ip and correctly close all sockets when jasm ends.
 */
struct module_running {
    pthread_t tid;
    char name[BUFSIZ];
    struct module_running *next;
};

struct ip_node {
    char client_ip[BUFSIZ];
    struct module_running *modules_list;
    struct ip_node *next;
};

extern int add_clientIp(struct ip_node **head, char *ip);
extern struct ip_node * find_clientIp(struct ip_node *head, char *ip);
extern int add_module_running(struct module_running **head, char *name, pthread_t tid);

extern int rem_clientIp(struct ip_node **head, char *ip);
extern int rem_module_running(struct module_running **head, char *name);

extern struct ip_node * find_clientIp(struct ip_node *head, char *ip);
extern struct module_running * find_module_running(struct module_running *head, char *name);

extern void print_ipList (struct ip_node *head);
extern void print_moduleList(struct module_running *head);
extern void print_all(struct ip_node *head);


#endif
