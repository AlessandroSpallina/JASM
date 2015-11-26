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
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <pthread.h>

#include "../core/modules.h"
#include "../core/miscellaneous.h"
#include "../core/queue.h"
#include "module_logsender.h"


static void send_next(int fd)
{
  char string[BUFSIZ];

  static FILE *fp=fopen(LOGPATH, "r");
  fscanf(fp, "%s", string);
  fclose(fp);

  send(fd, string, BUFSIZ);
}


/*
 *  ritorna 0 se tutto è ok, 1 se l'inizializzazione è fallita.
 *  il thread main dovrà comunicare al client che l'inizializzazione è andata
 *  storta :'(
 */
int init_logsender()
{
  // ***************** check file exists **************************************
  FILE *fp=fopen(LOGPATH, "r");
  if(fp!=NULL) {  //file exists: PERFECT
    fclose(fp);
    return 0;
  } else {
    log_error("[fail init logsender] unable to locate logfile :(")
    return 1;
  }
}


static void thread_routine(void *arg)
{
  struct running_module temp = {.name="LogSender", .tid=pthread_self};

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

  //aggiunge se stesso alla lista dei thread in esecuzione
  add((struct thread_arguments) arg.head, temp);

  while(1) {
    send_next((struct thread_arguments) arg.fd);
    sleep((struct thread_arguments) arg.sec);
  }
}

// ritorna 0 se tutto è ok, 1 se lo start è fallito.
int start_logsender(struct thread_arguments arg) //int sec = intervallo aggiornamento
{





}

void stop_logsender()
{
  //dealloca il thread
  pthread_exit(NULL);
}
