#ifndef _GETTER_H
#define _GETTER_H

#define NGETTER 7

extern char getterName[NGETTER][BUFSIZ];
extern void (*getterFunction[NGETTER])(int);

extern void getVersion(int fd);	//return jasm version
extern void getCopyright(int fd);	//return copyng license
extern void getHostname(int fd);
extern void getSysname(int fd);
extern void getSysrelease(int fd);
extern void getSysversion(int fd); 
extern void getMachine(int fd);

extern void getGetter(int fd);





#endif
