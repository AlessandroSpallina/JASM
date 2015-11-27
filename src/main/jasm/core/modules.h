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

struct running_module {
  char name[BUFSIZ];
  pthread_t tid;
  int *head;
  int fd;
  int sec;
  //struct running_module *next;
};

struct functions {
  void (*init) (void);  //inizializza l'ambiente del modulo e alloca in heap un elemento running_module (crea thread)
  void (*start)(int, int);  //e starta modulo
};

struct thread_arguments {
  struct running_module **head;
  int fd;
  int sec;
};

#endif
