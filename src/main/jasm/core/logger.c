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
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "miscellaneous.h"

//???
#ifdef DEBUG
#define CLIENTLOGPATH "../data/client.log"
#endif

static char LOGPATH[256] = "null";

void wlogev(const enum evtype ev, const char* strmsg)
{
	if(!strncmp(LOGPATH,"null",4)) {
		const char* home = getenv("HOME");
		strncpy(LOGPATH,home,strlen(home));
		strncat(LOGPATH,"/jasm.log",9);
	}

	FILE* flog = NULL;
	if(!(flog=fopen(LOGPATH,"a+")))
		return;

	char logtype[20];
	if(ev == EV_ERROR)
		strncpy(logtype,"ERROR",5);
	else if(ev == EV_WARN)
		strncpy(logtype,"WARNING",7);
	else 
		strncpy(logtype,"INFO",4);

	char curtime[256] = "null";
	get_time("%a %F %r",curtime);

	fprintf(flog,"[%s][%s] %s",logtype,curtime,strmsg);

	fclose(flog);
}

// ???
#ifdef DEBUG
void log_client (struct ip_node *clist)
{
								//NULLCHECK!!!
								FILE *fp = fopen(CLIENTLOGPATH, "a+");
								if (fp == NULL) {
									/* AVOID SEGFAULT !!! */
									return;
								}
								
								struct module_running *mlist = NULL;
								fprintf(fp, "=========\n[%s]\n", getTime());
								while(clist != NULL) {
																fprintf(fp, "* %s\n", clist->client_ip);
																mlist = clist->modules_list;
																while(mlist != NULL) {
																								fprintf(fp, "    @ %s\n", mlist->name);
																								mlist = mlist->next;
																}
																clist = clist->next;
								}
								fprintf(fp, "=========\n");
								if(fclose(fp) == -1) {
								  log_error("[JASM-DAEMON][ERROR][fclose()] Error while closing log_client");
								}
}
#endif
