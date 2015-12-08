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
#include <errno.h>

#include "getter.h"
#include "miscellaneous.h"

char error[BUFSIZ];

//TODO: checks + errno

void getVersion(int fd);
void getCopyright(int fd);
void getHostname(int fd);
void getGetter(int fd);

char getterName[NGETTER][BUFSIZ]={"Version", "Copyright", "Hostname", "KernelName",
                                  "KernelRelease", "KernelVersion", "Machine"};

void (*getterFunction[NGETTER])(int)={getVersion, getCopyright, getHostname,
                                      getKernelName, getKernelVersion, getKernelVersion, getMachine};

/*
 *  scrive su fd int numero getter e N stringhe nomiGetter
 */
void getGetter(int fd)
{
        int i;
        int ngetter = NGETTER;

        if(write(fd, &ngetter, sizeof(ngetter))<0)
        {
          sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
          log_error("[JASM-DAEMON][getGetter][write()] Error!");
          log_error(error);
        }
        for(i=0; i<NGETTER; i++) {
              if(write(fd, getterName[i], strlen(getterName[i])+1)<0)
              {
                sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
                log_error("[JASM-DAEMON][getGetter][write()] Error!");
                log_error(error);
              }
        }
}

void getVersion(int fd)
{
        if(write(fd, VERSION, strlen(VERSION)+1) < 0)
        {
          sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
          log_error("[JASM-DAEMON][getGetter][write()] Error!");
          log_error(error);
        }
}

void getCopyright(int fd)
{
        if(write(fd, COPYRIGHT, strlen(COPYRIGHT)+1) < 0)
        {
          sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
          log_error("[JASM-DAEMON][getGetter][write()] Error!");
          log_error(error);
        }
}

void getHostname(int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if(uname(&info)==-1) {
                log_error("getHostname() [uname] failed");
                return;
        } else {
                strcpy(buf, info.nodename);
                if(write(fd, buf, strlen(buf)+1) < 0)
                {
                  sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
                  log_error("[JASM-DAEMON][getGetter][write()] Error!");
                  log_error(error);
                }
        }
}

void getKernelName(int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if(uname(&info)==-1) {
                log_error("getKernelName() [uname] failed");
                return;
        } else {
                strcpy(buf, info.sysname);
                if(write(fd, buf, strlen(buf)+1)<0)
                {
                  sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
                  log_error("[JASM-DAEMON][getGetter][write()] Error!");
                  log_error(error);
                }
        }
}

void getKernelRelease(int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if(uname(&info)==-1) {
                log_error("getKernelRelease() [uname] failed");
                return;
        } else {
                strcpy(buf, info.release);
                if(write(fd, buf, strlen(buf)+1)<0)
                {
                  sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
                  log_error("[JASM-DAEMON][getGetter][write()] Error!");
                  log_error(error);
                }
        }
}

void getKernelVersion(int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if(uname(&info)==-1) {
                log_error("getKernelVersion() [uname] failed");
                return;
        } else {
                strcpy(buf, info.version);
                if(write(fd, buf, strlen(buf)+1)<0)
                {
                  sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
                  log_error("[JASM-DAEMON][getGetter][write()] Error!");
                  log_error(error);
                }
        }
}

void getMachine(int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if(uname(&info)==-1) {
                log_error("getMachine() [uname]  failed");
                return;
        } else {
                strcpy(buf, info.machine);
                if(write(fd, buf, strlen(buf)+1)<0)
                {
                  sprintf(error,"[JASM-DAEMON][errno] %s\n",strerror(errno));
                  log_error("[JASM-DAEMON][getGetter][write()] Error!");
                  log_error(error);
                }
        }
}
