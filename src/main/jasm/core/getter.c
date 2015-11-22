#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "getter.h"
#include "miscellaneous.h"
//all getters bring in queue in the heap mem. what they are going to do
//send_getter() to send the answer to the client

void getVersion(int fd);
void getCopyright(int fd);
void getHostname(int fd);
void getGetter(int fd);

char getterName[NGETTER][BUFSIZ]={"Version", "Copyright", "Hostname"};
void (*getterFunction[NGETTER])(int)={getVersion, getCopyright, getHostname};

/*
 *  scrive su fd int numero getter e N stringhe nomiGetter
 */
void getGetter(int fd)
{
  int i;
  int ngetter = NGETTER;

  write(fd, &ngetter, sizeof(ngetter));
  for(i=0; i<NGETTER; i++) {
    write(fd, getterName[i], BUFSIZ);
  }
}

void getVersion(int fd)
{
  write(fd, VERSION, BUFSIZ);
}

void getCopyright(int fd)
{
  write(fd, COPYRIGHT, BUFSIZ);
}

void getHostname(int fd)
{
  char buf[BUFSIZ];

  if(gethostname(buf, sizeof(char)*BUFSIZ)==-1) {
    log_error("gethostname fail");
    return;
  } else {
    write(fd, buf, BUFSIZ);
  }
}
