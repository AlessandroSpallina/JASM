#ifndef _GETTER_H
#define _GETTER_H

#define NGETTER 3

extern char getterName[NGETTER][BUFSIZ];
extern void (*getterFunction[NGETTER])(int);

extern void getVersion(int fd);	//return jasm version
extern void getCopyright(int fd);	//return copyng license
extern void getHostname(int fd);
extern void getGetter(int fd);





#endif
