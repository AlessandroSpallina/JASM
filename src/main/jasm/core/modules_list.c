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

/*
 *  Return values:
 *    -1) malloc fail
 *     0) element added with success
 */
int add_module_running(struct module_running **head, char *ip, char *name, pthread_t tid) {

  struct module_running *aus = NULL;

  aus = (struct module_thread *) malloc (sizeof (struct module_thread));
  if(aus == NULL)
    return -1;

  strcpy(aus->client_ip, ip);
  strcpy(aus->name, name);
  aus->tid = tid;


}
