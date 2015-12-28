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
#ifndef _MODULES_H
#define _MODULES_H

#define NMODULE 1

/*
 *  this struct is the atomic element of a ordered list allocated in heap.
 *  the list is ordered by client_ip. this help us to maintain and allow
 *  only one instance of any module for client_ip and correctly close
 *  all sockets when jasm ends.
 */
struct module_running {
    char client_ip[BUFSIZ];
    pthread_t tid;
    char name[BUFSIZ];
    struct module_running *next;
};

//this array cointains modules in execution on threads
extern struct module_running module_table[NMODULE];

extern char moduleName[NMODULE][BUFSIZ];
extern void (*moduleInit[NMODULE]) (int, int);
extern void (*moduleStart[NMODULE]) (void);

extern void getModule (int fd);

#endif
