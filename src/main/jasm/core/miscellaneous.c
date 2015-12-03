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
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <crypt.h>

#include "miscellaneous.h"

char *getTime()
{
        time_t curtime;
        struct tm *loctime;
        static char *ret;

        curtime=time(NULL);
        loctime=localtime(&curtime);
        ret=asctime(loctime);
        ret[24]='\0';

        return ret;
}

void log_string(const char *message)
{
        #ifdef LOGPATH
        //log in LOGPATH
        #else
        char LOGPATH[BUFSIZ];
        sprintf(LOGPATH,"%s/.jasm.log",getenv("HOME"));
        #endif

        FILE *fp;

        if((fp=fopen(LOGPATH, "a+")) == NULL) {
                fprintf(fp,"[%s][INFO]This file is created now.",getTime());
        } else {
                fprintf(fp, "[%s][INFO] %s\n", getTime(), message);
                fclose(fp);
        }
}

void log_error(const char *message)
{
        #ifdef LOGPATH
        //log in LOGPATH
        #else
        char LOGPATH[BUFSIZ];
        sprintf(LOGPATH,"%s/.jasm.log",getenv("HOME"));
        #endif

        FILE *fp;

        if((fp=fopen(LOGPATH, "a+")) == NULL) {
                fprintf(fp,"[%s][INFO] This file is created now.",getTime());
        } else {
                fprintf(fp, "[%s][ERROR] %s!\n", getTime(), message);
                fclose(fp);
        }
}

void start_daemon()
{

        pid_t pid;
        char buf[BUFSIZ];

        log_string("[JASM-DAEMON] success!");
        pid=fork();
        switch(pid) {
        case -1:
                log_error("[THREAD-SPAWN][fork()] failed");
                exit(1);
                break;

        case 0:
                log_string("[THREAD-SPAWN][fork()] success");
                break;

        default:
                exit(0);
                break;
        }
        if(setsid()<0) {
                log_error("[THREAD-BACKGROUND][setsid()] failed");
                exit(1);
        } else {
                log_string("[THREAD-BACKGROUND][setsid()] success");
        }
        //closes fd: stdin, stdout, stderr
        close(0);
        close(1);
        close(2);
        sprintf(buf, "[JASM-DAEMON][START] PID: %d , Parent PID: %d", getpid(), getppid());
        log_string(buf);
}

/*LOGIN SECTION*/
int login_required(const char* clientaddr)
{
	if(strcmp(clientaddr,LOCALHOST) == 0) return 0;
	else return 1;
}

int check_passwd_file(const char* __pwdf)
{
	if(access(__pwdf,F_OK) != -1) return 0;
  else return 1;
}
