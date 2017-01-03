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

#ifndef _MACROS_H
#define _MACROS_H

#define _EXIT_SUCCESS 0
#define SOCKET_CREATION_FAILED 120
#define SOCKET_BINDING_FAILED 122
#define SOCKET_SELECT_FAILED 123
#define SOCKET_LISTENING_CONNECTION_FAILED 124
#define NOFILE_ERROR 5
#define ERR_SET_PROCESS_BACKGROUND 140
#define ERR_SET_PROCESS_SPAWN 141

#define LOCALHOST "127.0.0.1"
#define MAX_LOG_CHARS 100
#define NMODULE 1
#define NGETTER 37
#define MAX_LENGHT_RECV 256
#define MAX_LENGHT_SEND 256
#define SERVER_PORT 9734
#define MAX_DATA_TYPE_STRSIZE 20

#define DATA_TEXT "data/text"
#define DATA_NODATA "data/nodata"

#define PROTO_VERSION "0.0.1"

#endif
