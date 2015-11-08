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
#include "miscellaneous.h"

char * getTime()
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
  FILE *fp;

  fp=fopen(LOGPATH, "a+");
  fprintf(fp, "[%s] %s\n", getTime(), message);
  fclose(fp);
}

void log_error(const char *message)
{
  FILE *fp;

  fp=fopen(LOGPATH, "a+");
  fprintf(fp, "[%s] ERROR: %s!\n", getTime(), message);
  fclose(fp);
}

void start_daemon()
{
  pid_t pid;
  char buf[BUFSIZ];

  log_string("boot");

  pid=fork();
  switch(pid) {
    case -1:
      log_error("fork fail");
      exit(1);
      break;

    case 0:
      log_string("fork success");
      break;

    default:
      exit(0);
      break;
  }

  if(setsid()<0) {
    log_error("setsid fail");
    exit(1);
  } else {
    log_string("setsid success");
  }

  //chiude i file descriptor di stdin, stdout, stderr
  close(0);
  close(1);
  close(2);

  sprintf(buf, "jasm started with pid %d and ppid %d", getpid(), getppid());

  log_string(buf);
}
