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
#define SOCKET_CREATION_FAILED 120
#define SOCKET_CLIENT_CONNECTION_FAILED 121
#define SOCKET_BINDING_FAILED 122
#define SOCKET_SELECT_FAILED 123
#define SOCKET_LISTENING_CONNECTION_FAILED 124
#define NOFILE_ERROR 5
#define ERR_SET_PROCESS_BACKGROUND 140
#define ERR_SET_PROCESS_SPAWN 141
#define LOCALHOST "127.0.0.1"
#define LOGFILENAME "jasm.log"
#define LOGPATH "../data/jasm.log"
#define VERSION "jasm-develstage" //<-- CHANGE IT!
#define COPYRIGHT "GNU Public License (GPL) v3" //<-- GPLv3

extern char buildate[256];

extern char *getTime(void);
extern void log_string(const char *message);
extern void log_error(const char *message);
extern void start_daemon(void);
extern int login_required(const char *clientaddr);
extern int check_passwd_file(const char* __pwdf);

#endif
