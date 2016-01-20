#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "miscellaneous.h"

//CHECK LOGPATH

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
