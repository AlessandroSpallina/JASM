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
#include "miscellaneous.h"
#include "ipc.h"
#include "signals.h"
#include "logger.h"
#include "cfgparser.h"

int main (void)
{
	struct configval cfgvalues;

	set_logpath(); //sets path where to log..
	start_daemon(); //starts background daemon
	set_signals_feel(); //logs a set of signals
    if(get_values(&cfgvalues) == -1) {
        log_string("[JASM-DAEMON][CONFIG] Configuration file doesn't exist!");
    }

    //starts server after the daemon (ready to get commands)
	start_server();
}
