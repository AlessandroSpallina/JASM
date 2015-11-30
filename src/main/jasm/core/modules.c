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

#include "../modules/module_logsender.h"
#include "modules.h"

char moduleName[NMODULE][BUFSIZ]={"logsender"};

void assign_modules_functions(struct functions *modules_functions[NMODULE])
{
	modules_functions = malloc(NMODULE * sizeof(*modules_functions));
	modules_functions[0]->init=init_mod_logsender;
	modules_functions[0]->start=start_mod_logsender;
}

void assign_modules_properties(struct running_module *run_mod_prop[NMODULE])
{
	run_mod_prop = malloc(NMODULE * sizeof(*run_mod_prop));
	run_mod_prop[0]->name[BUFSIZ]=(char*)moduleName[0];
	//run_mod_prop[0]->sec=0;
}
