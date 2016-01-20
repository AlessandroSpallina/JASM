#ifndef LOGGER_H
#define LOGGER_H

extern char LOGPATH[BUFSIZ];

extern void log_string(const char * message);
extern void log_error(const char * message);

#endif //LOGGER_H
