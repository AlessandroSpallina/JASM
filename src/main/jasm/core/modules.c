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
#include <pthread.h>
#include "modules.h"
#include "../modules/module_logsender.h"

char moduleName[NMODULE][BUFSIZ] = {"Logsender"};
void (*moduleInit[NMODULE])(int, int) = {init_logsender};
void (*moduleStart[NMODULE])(void) = {start_logsender};

void getModule(int fd)
{
  int count=0;
  int nmodule = NMODULE;

  write(fd, &nmodule, sizeof(nmodule));

  for(int i=0; i<NMODULE; i++) {
    count = strlen(moduleName[i]);

    //aggiungere check errore nell'invio
    write(fd, &count, sizeof(count));
    write(fd, moduleName[i], strlen(moduleName[i]));
  }

}
