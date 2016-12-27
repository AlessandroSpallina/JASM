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
#include <unistd.h>

#ifdef __unix__
#include <errno.h>
#endif

#include "modules.h"
#include "logger.h"
#include "ipc.h"

char errlog[BUFSIZ];
char moduleName[NMODULE][BUFSIZ] = {};
void (*moduleInit[NMODULE]) (int, int) = {};
void (*moduleStart[NMODULE]) (void) = {};
void (*moduleEnd[NMODULE]) (void) = {};

void getModule (int fd)
{
        size_t count = 0;
        int nmodule = NMODULE;
        ssize_t ret_val = 0;
        char conv_value[NMODULE];

        sprintf(conv_value, "%d", nmodule);
        ret_val = sendMsg (fd, conv_value);
        if (ret_val == 0 || ret_val == -1)
        {
#ifdef DEBUG
                sprintf(errlog,"[JASM-DAEMON][ERROR][write()] Error: %s",strerror(errno));
                wlogev(EV_ERROR, errlog);
#endif
                wlogev(EV_ERROR, "[JASM-DAEMON][ERROR][write()] Error while sending module list");
        }

        for (int i = 0; i < NMODULE; i++)
        {
                count = strlen (moduleName[i]);

                sprintf(conv_value, "%d", count);
                ret_val = sendMsg (fd, conv_value);
                if (ret_val == 0 || ret_val == -1) {
                        fprintf (stderr, "Error in the write() operation on fd");
                }
                sendMsg(fd,moduleName[i]);
        }
}
