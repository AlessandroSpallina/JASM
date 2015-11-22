#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "getter.h"
#include "miscellaneous.h"
//all getters bring in queue in the heap mem. what they are going to do 
//send_getter() to send the answer to the client

void getVersion(int fd);
void getCopyright(int fd);

char getterName[NGETTER][BUFSIZ]={"Version", "Copyright"};
void (*getterFunction[NGETTER])(int)={getVersion, getCopyright};

void getVersion(int fd)
{
  write(fd, VERSION, BUFSIZ);
}

void getCopyright(int fd)
{
  write(fd, COPYRIGHT, BUFSIZ);
}
