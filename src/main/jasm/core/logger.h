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
#ifndef LOGGER_H
#define LOGGER_H

#ifdef DEBUG
  #define CLIENTLOGPATH "../data/client.log"
#endif

extern char LOGPATH[BUFSIZ];
extern char homedir[BUFSIZ];

extern void log_string(const char * message);
extern void log_error(const char * message);
extern void set_logpath(void);

#ifdef DEBUG
  extern void log_client (struct ip_node *clist);
#endif

#endif //LOGGER_H
