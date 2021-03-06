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
#include <string.h>
/*#include <sys/utsname.h>
#include <pthread.h>*/
#include <errno.h>

//#include "../core/modules.h"
#include "../core/logger.h"
#include "../core/ipc.h"

#include "module_logsender.h"

/*
 * BUGGY!
 */

char errlog[BUFSIZ];

static int fd = -1;
static int sec = -1;

//this function is the core of this module
static int sendNextLine(FILE **fp)
{
        char *line = NULL;
        size_t len = 0;
        ssize_t ret;

        if((ret = getline(&line, &len, (*fp))) == -1) {
                free(line);
                return -1;
        }

        if(write(fd, &ret, sizeof(ret)) < 0){
#ifdef DEBUG
          sprintf(errlog,"[MODULE][Logsender] Error in first write : %s",strerror(errno));
          log_error(errlog);
#endif
          log_error("[MODULE][Logsender] Error while sending!");
        }
        sendMsg(fd,line);
        free(line);
        return 0;
}

//this function should be the first to be call before start the module
void init_logsender(int filedescr, int seconds)
{
        fd = filedescr;
        sec = seconds;
        log_string("[MODULE][Logsender] Successfully initialized!");
}


//this function will be on a new jasm thread
void start_logsender()
{
        if(sec == -1 || fd == -1) {
                log_error("[MODULE][Logsender] Hasn't been initialized!");
                return;
        }


        FILE *fp;
        if((fp = fopen(LOGPATH, "r")) != NULL){
          /*NONE, JUST CHECK*/
        }

        //sends all log file
        while(sendNextLine(&fp) != -1) ;

        while(1) { /* ENDLESS LOOP */
                //and wait n sec before updating :)
                //The sleep() function shall cause the calling thread to be suspended from execution [POSIX Doc]
                sleep(sec);
                sendNextLine(&fp);
        }
}
