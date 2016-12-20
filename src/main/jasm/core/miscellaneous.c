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
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#ifdef __unix__
#include <syslog.h>
#include <errno.h>
#endif

#include "jasmbuild_info.h"
#include "miscellaneous.h"
#include "logger.h"
#include "macros.h"

static char errlog[MAX_LOG_CHARS];
static char buildate[256] = "null";

void get_buildate(char *dest)
{
    if(!strncmp(buildate,"null",4))
#ifdef BUILD_DATE_CORE
        strncpy(buildate,BUILD_DATE_CORE,strlen(BUILD_DATE_CORE));
#endif
    strncpy(dest,buildate,strlen(buildate)+1);
}

void get_time(const char* format, char* dest)
{
    char dest_time[256];
    memset(dest,0,strlen(dest));

    time_t curtime;
    struct tm *loctime;

    curtime = time (NULL);
    loctime = localtime (&curtime);

    if(!loctime)
        return;

    if(!strftime(dest_time,sizeof(dest_time),format,loctime))
        return;

    strncpy(dest,dest_time,strlen(dest_time)); //not null-terminated
}

void start_daemon()
{
        pid_t pid;

        snprintf (errlog, MAX_LOG_CHARS, "JASM System Monitor Starting Up... Version: %s , Build Date: %s", VERSION, buildate);
        wlogev(EV_INFO,errlog);

#ifdef DEBUG
        wlogev(EV_WARN,"You are using JASM debug build");
#endif

        pid = fork();
        switch (pid) {
        case -1:
                snprintf (errlog, MAX_LOG_CHARS, "forking error: %s\n", strerror (errno) );
                wlogev(EV_ERROR,errlog);

                openlog ("JASM", LOG_PID, LOG_DAEMON);
                syslog (LOG_ERR, "Process spawning failed!");
                closelog();
                exit (ERR_SET_PROCESS_SPAWN);
        case 0:
                break;
        default:
                exit (_EXIT_SUCCESS);
        }

        if (setsid() < 0) {
                snprintf (errlog, MAX_LOG_CHARS,"getting new sid failure: %s\n", strerror (errno) );
                wlogev(EV_ERROR,errlog);
                openlog ("JASM", LOG_PID, LOG_DAEMON);
                syslog (LOG_ERR, "Setting sid for new process failed!");
                closelog();
                exit (ERR_SET_PROCESS_BACKGROUND);
        } else {
            log_string ("[PROCESS-SID][setsid()] success");
        }

        //closes fd: stdin, stdout, stderr
        close (0);
        close (1);
        close (2);

        snprintf (errlog, MAX_LOG_CHARS,"PID: %d , Parent PID: %d", getpid(), getppid() );
        wlogev(EV_INFO,errlog);
        wlogev(EV_INFO,"JASM is now ready to get commands! :)");

        openlog ("JASM", LOG_PID, LOG_DAEMON);
        syslog (LOG_INFO, "SUCCESS! New jasm process created! READY!");
        closelog();
}

_Bool login_required (const char* clientaddr)
{
    if(!clientaddr)
        return false;

    return strncmp(clientaddr,LOCALHOST,strlen(clientaddr)) > 0 ? true : false;
}

_Bool check_passwd_file (const char* __pwdf)
{
    return access(__pwdf,F_OK) != -1 ? true : false;
}

int read_line(const int file, char *buffer, const int length)
{
   int count = 0, run=1;
   ssize_t res;

   while(run) {
       res = read(file, &(buffer[count]), 1);
       if(res == 0 && count == 0)
           return 0;
       if(res == 0 || buffer[count] == '\n' || count == length)
           run = 0;
       count++;
   }

   buffer[count-1] = '\0';
   return count;
}
