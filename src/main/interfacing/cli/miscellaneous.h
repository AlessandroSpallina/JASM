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

#define _EXIT_SUCCESS 0

#define ARG_SPECIFY_IPADDR 137

#define PASSWD_REFUSED_ERROR 10

#define NOPASSWD_SET_JASM 20

#define SOCKET_CREATION_FAILED 120
#define SOCKET_CONNECTION_FAILED 121

#define LOGIN_TOO_MUCH_ATTEMPTS 190

#define SERVER_DISCONNECTED 190

extern char color[4][BUFSIZ];
extern char buildtime[BUFSIZ];
extern char debugstr[BUFSIZ];

extern void log_string(const char *message);
extern char* getTime(void);
extern void check_debug(void);
extern void check_release(void);
extern int check_if_file_exists(const char * __fname);

#endif
