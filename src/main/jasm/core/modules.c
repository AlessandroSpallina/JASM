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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __unix__
#include <pthread.h>
#include <errno.h>
#endif

#include "miscellaneous.h"
#include "modules.h"
#include "logger.h"
#include "../modules/module_logsender.h"

char errlog[BUFSIZ];
char moduleName[NMODULE][BUFSIZ] = {"Logsender"};
void (*moduleInit[NMODULE]) (int, int) = {init_logsender};
void (*moduleStart[NMODULE]) (void) = {start_logsender};

void getModule (int fd)
{
        int count = 0;
        int nmodule = NMODULE;
        ssize_t ret_val = 0;

        ret_val = write (fd, &nmodule, sizeof (nmodule) );
        if (ret_val == 0 || ret_val == -1)
        {
#ifdef DEBUG
                sprintf(errlog,"[JASM-DAEMON][ERROR][write()] Error: %s",strerror(errno));
                log_error(errlog);
#endif
                log_error("[JASM-DAEMON][ERROR][write()] Error while sending module list");
        }

        for (int i = 0; i < NMODULE; i++)
        {
                count = strlen (moduleName[i]);

                ret_val = write (fd, &count, sizeof (count) );
                if (ret_val == 0 || ret_val == -1)
                {
                        fprintf (stderr, "Error in the write() operation on fd");
                }
                if (write (fd, moduleName[i], count) <0) {
#ifdef DEBUG
                        sprintf(errlog,"[JASM-DAEMON][ERROR][write()] Error: %s",strerror(errno));
                        log_error(errlog);
#endif
                        log_error("[JASM-DAEMON][ERROR][write()] Error while sending module list");
                }
        }
}
