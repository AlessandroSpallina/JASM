#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#include "miscellaneous.h"

static void signal_handler(int sigx)
{
  printf("* Recieved signal: %d\n",sigx);
}

void signal_catcher(void)
{
  struct sigaction act;

  act.sa_handler=signal_handler;
  act.sa_flags=0;

  sigaction(SIGKILL, &act, 0);
  sigaction(SIGSTOP, &act, 0);
  sigaction(SIGQUIT, &act, 0);
  sigaction(SIGTERM, &act, 0);
  sigaction(SIGINT, &act, 0);
}
