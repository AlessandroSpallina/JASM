#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/utsname.h>
#include "getter.h"
#include "miscellaneous.h"

//all getters bring in queue in the heap mem. what they are going to do
//send_getter() to send the answer to the client

void getVersion(int fd);
void getCopyright(int fd);
void getHostname(int fd);
void getGetter(int fd);

char getterName[NGETTER][BUFSIZ]={"Version", "Copyright", "Hostname", "Sysname",
  "Sysrelease", "Sysversion", "Machine"};

void (*getterFunction[NGETTER])(int)={getVersion, getCopyright, getHostname,
  getSysname, getSysrelease, getSysversion, getMachine};

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
    log_error("getHostname() [gethostname] failed");
    return;
  } else {
    write(fd, buf, BUFSIZ);
  }
}

void getSysname(int fd)
{
  struct utsname info;
  char buf[BUFSIZ];

  if(uname(&info)==-1) {
    log_error("getSysname() [uname] failed");
    return;
  } else {
    strcpy(buf, info.sysname);
    write(fd, buf, BUFSIZ);
  }
}

void getSysrelease(int fd)
{
  struct utsname info;
  char buf[BUFSIZ];

  if(uname(&info)==-1) {
    log_error("getSysrelease() [uname] failed");
    return;
  } else {
    strcpy(buf, info.release);
    write(fd, buf, BUFSIZ);
  }
}

void getSysversion(int fd)
{
  struct utsname info;
  char buf[BUFSIZ];

  if(uname(&info)==-1) {
    log_error("getSysversion() [uname] failed");
    return;
  } else {
    strcpy(buf, info.version);
    write(fd, buf, BUFSIZ);
  }
}

void getMachine(int fd)
{
  struct utsname info;
  char buf[BUFSIZ];

  if(uname(&info)==-1) {
    log_error("getMachine() [uname]  failed");
    return;
  } else {
    strcpy(buf, info.machine);
    write(fd, buf, BUFSIZ);
  }
}
