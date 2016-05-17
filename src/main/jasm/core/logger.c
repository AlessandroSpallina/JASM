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

#include "modules_list.h"
#include "logger.h"
#include "miscellaneous.h"


char homedir[BUFSIZ];
char LOGPATH[BUFSIZ];

void set_logpath(void)
{
								strcpy(homedir,getenv("HOME"));
								strcpy(LOGPATH,homedir);
								strcat(LOGPATH,"/.jasm.log");
}

void log_string (const char *message)
{
								FILE *fp;

								if ( (fp = fopen (LOGPATH, "a+") ) == NULL) {

																fprintf (fp, "[%s][INFO]This file is created now.", getTime() );
								} else {
																fprintf (fp, "[%s][INFO] %s\n", getTime(), message);
																fclose (fp);
								}
}

void log_error (const char *message)
{
								FILE *fp;

								if ( (fp = fopen (LOGPATH, "a+") ) == NULL) {
                                                                fprintf (fp, "[%s][INFO] This file is created now.", getTime() );
								} else {
																fprintf (fp, "[%s][ERROR] %s!\n", getTime(), message);
																fclose (fp);
								}
}

#ifdef DEBUG
void log_client (struct ip_node *clist)
{

								FILE *fp = fopen(CLIENTLOGPATH, "a+");
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
								fclose(fp);
								fprintf(fp, "=========\n");
}
#endif
