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
#include <signal.h>
#include <stdio.h>

#include "logger.h"
#include "signals.h"
#include "macros.h"

static void generic_signal_log (int sig)
{
   char errlog[MAX_LOG_CHARS];

   snprintf(errlog,MAX_LOG_CHARS,"Received signal number %d", sig);
   wlogev(EV_WARN,errlog);
}

void set_signals_feel()
{
   struct sigaction act;

   act.sa_handler = generic_signal_log;
   act.sa_flags = 0;
   sigaction (SIGINT, &act, 0);
   sigaction (SIGQUIT, &act, 0);
   sigaction (SIGCONT, &act, 0);
   sigaction (SIGSTOP, &act, 0);
}
