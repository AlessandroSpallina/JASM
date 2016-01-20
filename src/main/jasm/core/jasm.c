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
#include "logger.h"

int main (int argc, char *argv[])
{

#ifdef DEBUG
        char logstr_debug[BUFSIZ];
#endif //DEBUG

        int propretval;

        start_daemon(); //starts background daemon
        set_signals_feel(); //logs a set of signals
        propretval = set_property_value();

        if (propretval == 1) {
                log_string ("[JASM-DAEMON][INFO]You need to create a configuration file");
                log_string ("[JASM-DAEMON][INFO]Using standard values");
                log_string ("[JASM-DAEMON][INFO]Server's config source: $HOME/.jasm_config");
        } else if (propretval == -1) log_string ("[JASM-DAEMON][INFO]Property value is NULL!");
        else
                log_string ("[JASM-DAEMON][INFO]Using values defined in the server's configuration file!");
#ifdef DEBUG
                log_string ("[JASM-DAEMON][INFO][DEBUG] Accessing struct...");
                for(int ind=0; ind<=NCONFIG_PROPERTIES_COUNTER; ind++) {
                        sprintf(logstr_debug,"[JASM-DAEMON][INFO][DEBUG] Key: %s , Value: %d",_config[ind].config_name, *(int*)_config[ind].config_values);
                        log_string(logstr_debug);
                }
#endif //DEBUG
        start_server(); //starts server after the daemon (ready to get commands)
}
