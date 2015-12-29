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
#include "miscellaneous.h"
#include "ipc.h"
#include "signals.h"
#include "configfile.h"
#include "modules_list.h"

int main (int argc, char *argv[])
{
        start_daemon(); //starts background daemon
        set_signals_feel(); //logs a set of signals

        if (set_property_value() == 1) {
                log_string ("[JASM-DAEMON][INFO]You need to create a configuration file");
                log_string ("[JASM-DAEMON][INFO]Using standard values");
                log_string ("[JASM-DAEMON][INFO]Server's config source: $HOME/.jasm_config");
        } else {
                log_string ("[JASM-DAEMON][INFO]Using values defined in the server's configuration file!");
        }
        start_server(); //starts server after the daemon (ready to get commands)

        /*struct module_running *genesi = NULL;
        printf("%d value\n", add_module_running(&genesi, "ciao", 1111));
        printf("%d value\n", add_module_running(&genesi, "alessandro", 1111));
        print_moduleList(genesi);
        printf("%d value\n", add_module_running(&genesi, "bau", 1111));
        print_moduleList(genesi);
        printf("%d value\n", add_module_running(&genesi, "zio", 1111));

        print_moduleList(genesi);*/

}
