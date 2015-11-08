#ifndef _MODULES_H
#define _MODULES_H

struct running_modules {
  char name[BUFSIZ];
  pthread_t tid;
};

#endif
