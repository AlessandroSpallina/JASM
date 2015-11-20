#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "getter.h"
#include "miscellaneous.h"
//tutti i getter mettono in coda in heap ciò che devono e sarà poi la funzione
//send_getter() ad inviare la risposta al client

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
