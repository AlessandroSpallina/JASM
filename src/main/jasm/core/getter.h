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
#ifndef _GETTER_H
#define _GETTER_H

#define NGETTER 7

extern char getterName[NGETTER][BUFSIZ];
extern void (*getterFunction[NGETTER])(int);

extern void getVersion(int fd); //return jasm version
extern void getCopyright(int fd); //return copyng license
extern void getHostname(int fd);
extern void getKernelName(int fd);
extern void getKernelRelease(int fd);
extern void getKernelVersion(int fd);
extern void getMachine(int fd);
extern void getGetter(int fd);

#endif
