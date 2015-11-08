#ifndef _MISCELLANEOUS_H
#define _MISCELLANEOUS_H

#define LOGPATH "../../../../data/log/jasm.log"
#define LOGFILENAME "jasm.log"
#define VERSION "0.2"

extern char *getTime(void);
extern void log_string(const char *message);
extern void log_error(const char *message);
extern void start_daemon(void);

#endif
