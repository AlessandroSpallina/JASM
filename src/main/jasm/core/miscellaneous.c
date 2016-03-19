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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <errno.h>
#endif

#include "jasmbuild_info.h"
#include "configfile.h"
#include "miscellaneous.h"
#include "logger.h"

char errlog[BUFSIZ]; // pattern to follow: [SECTION][ERROR]Errno: %s ...

char buildate[256] = "null";

char *getTime()
{
        time_t curtime;
        struct tm *loctime;
        static char *ret;

        curtime = time (NULL);
        loctime = localtime (&curtime);
        ret = asctime (loctime);
        ret[24] = '\0';

        return ret;
}

void check_buildate()
{
#ifdef BUILD_DATE_CORE
        strcpy (buildate, BUILD_DATE_CORE);
#else
        strcpy (buildate, "not availible");
#endif
}

void start_daemon()
{
        check_buildate();

        pid_t pid;
        char buf[BUFSIZ];
        char buf_intro[BUFSIZ];

        sprintf (buf_intro, "[START]JASM System Monitor Starting Up... Version: %s , Build Date: %s", VERSION, buildate);

        log_string ("=======================================");
#ifdef DEBUG
        log_string ("[BUILD] You are using JASM debug build!");
#endif

        log_string (buf_intro);
        log_string ("[JASM-DAEMON] Creating new process...");
        pid = fork();
        switch (pid)
        {
        case -1:
                sprintf (errlog, "[PROCESS-SPAWN]Error: %s\n", strerror (errno) );
                log_error ("[PROCESS-SPAWN][fork()] failed");
                log_error (errlog);
                openlog ("JASM", LOG_PID, LOG_DAEMON);
                syslog (LOG_ERR, "Process spawning failed!");
                closelog();
                exit (ERR_SET_PROCESS_SPAWN);
                break;

        case 0:
                log_string ("[PROCESS-SPAWN][fork()] success");
                break;

        default:
                exit (_EXIT_SUCCESS);
                break;
        }
        if (setsid() < 0)
        {
                log_error ("[PROCESS-BACKGROUND][setsid()] failed");
                sprintf (errlog, "[PROCESS-BACKGROUND]Error: %s\n", strerror (errno) );
                log_error (errlog);
                openlog ("JASM", LOG_PID, LOG_DAEMON);
                syslog (LOG_ERR, "Put process background failed!");
                closelog();
                exit (ERR_SET_PROCESS_BACKGROUND);
        }
        else log_string ("[PROCESS-BACKGROUND][setsid()] success");

        //closes fd: stdin, stdout, stderr
        close (0);
        close (1);
        close (2);

        sprintf (buf, "[JASM-DAEMON][START] PID: %d , Parent PID: %d", getpid(), getppid() );
        log_string (buf);
        log_string ("[JASM-DAEMON][START] Start phase successfully completed!");
        log_string ("[JASM-DAEMON][STATUS] Ready!");
        openlog ("JASM", LOG_PID, LOG_DAEMON);
        syslog (LOG_INFO, "SUCCESS! New jasm process created! READY!");
        closelog();
}

/*LOGIN SECTION*/
_Bool login_required (const char* clientaddr)
{
        if (strcmp (clientaddr, LOCALHOST) == 0) return false;
        else return true;
}

int check_passwd_file (const char* __pwdf)
{
        if (access (__pwdf, F_OK) != -1) return 0;
        else return 1;
}

int read_line(int file, char *buffer,int length) {
  int count = 0, run = 1;
  int res;
  while(run) {
    res = read(file, &(buffer[count]), 1);
    if(res == 0 && count == 0) return 0;
    if(res == 0 || buffer[count] == '\n' || count == length) run = 0;
    count++;
  }
  buffer[count-1] = '\0';
  return count;
}
