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
#ifndef _MISCELLANEOUS_H
#define _MISCELLANEOUS_H

#include <sys/types.h>
#include <stdbool.h>

ssize_t read(int fd, void* buf, size_t length);

extern void get_time (const char* format, char* dest);
extern void start_daemon (void);
extern bool login_required (const char *clientaddr);
extern bool check_passwd_file (const char* __pwdf);
extern int read_line(const int file, char *buffer, const int length);

#endif
