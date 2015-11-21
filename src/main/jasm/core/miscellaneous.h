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

#define LOGPATH "/var/log/jasm.log"
#define LOGFILENAME "jasm.log"

#define VERSION "JustAnotherSystemMonitor v0.2"
#define COPYRIGHT "GNU Public License v3"

extern char *getTime(void);
extern void log_string(const char *message);
extern void log_error(const char *message);
extern void start_daemon(void);

#endif
