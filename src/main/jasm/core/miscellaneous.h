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

ssize_t read(int fd, void* buf, size_t length);

extern void get_buildate(char* dest);
extern void get_time (const char* format, char* dest);
extern void start_daemon (void);
extern _Bool login_required (const char *clientaddr);
extern _Bool check_passwd_file (const char* __pwdf);
extern inline int read_line(const int file, char *buffer, const int length) {
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

#endif
