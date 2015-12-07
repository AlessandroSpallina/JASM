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
#include <time.h>
#include <string.h>

char buildtime[BUFSIZ]="null";
char debugstr[BUFSIZ]="null";

char color[4][BUFSIZ]={
  "\033[32m", //green
  "\033[31m", //red
  "\033[33m", //yellow
  "\033[34m" //blue
};

void log_string(const char *message)
{
  printf("# Sys: %s\n", message);
}

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

void check_debug()
{
  #ifdef DEBUG
  strcpy(debugstr,"* You are using JASMCLI *debug* build!");
  #else
  strcpy(debugstr,"\0");
  #endif
}

void check_release()
{
  #ifdef BUILD_DATE_CLI
  strcpy(buildtime,BUILD_DATE_CLI);
  #else
  strcpy("not availible",BUILD_DATE_CLI);
  #endif
}
