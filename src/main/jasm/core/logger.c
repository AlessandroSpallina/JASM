#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "miscellaneous.h"

char LOGPATH[BUFSIZ];

void set_logpath(void) {
#ifdef DEBUG
strcpy(LOGPATH,"../data/jasm.log");
#else
strcpy(LOGPATH,"home");
strcat(LOGPATH,"/.jasm.log");
#endif //DEBUG
}

void log_string (const char *message)
{
    FILE *fp;

    if ( (fp = fopen (LOGPATH, "a+") ) == NULL)
    {
        fprintf (fp, "[%s][INFO]This file is created now.", getTime() );
    }
    else
    {
        fprintf (fp, "[%s][INFO] %s\n", getTime(), message);
        fclose (fp);
    }
}

void log_error (const char *message)
{
    FILE *fp;

    if ( (fp = fopen (LOGPATH, "a+") ) == NULL)
    {
        fprintf (fp, "[%s][INFO] This file is created now.", getTime() );
    }
    else
    {
        fprintf (fp, "[%s][ERROR] %s!\n", getTime(), message);
        fclose (fp);
    }
}
