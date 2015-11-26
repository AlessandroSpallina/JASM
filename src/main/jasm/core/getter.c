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
#include "getter.h"
#include "miscellaneous.h"

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

        write(fd, &ngetter, sizeof(ngetter));
        for(i=0; i<NGETTER; i++) {
                write(fd, getterName[i], BUFSIZ);
        }
}

void getVersion(int fd)
{
        write(fd, VERSION, BUFSIZ);
}

void getCopyright(int fd)
{
        write(fd, COPYRIGHT, BUFSIZ);
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
                write(fd, buf, BUFSIZ);
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
                write(fd, buf, BUFSIZ);
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
                write(fd, buf, BUFSIZ);
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
                write(fd, buf, BUFSIZ);
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
                write(fd, buf, BUFSIZ);
        }
}
