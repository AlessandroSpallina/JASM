#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "miscellaneous.h"
#include "configfile.h"

struct config _config[NCONFIG_PROPERTIES]; //access data through this struct

char * get_property_value (char * __prop)
{
    strtok (__prop, "=");
    return strtok (NULL, "=");
}

void set_default_property_value (int what)
{
    if (what == CONFIG_ALL)
    {
        _config[CONFIG_MAX_CONNECTIONS].config_values = (int*) 3;
        _config[CONFIG_MAX_AUTHENTICATION_TRIES].config_values = (int*) 5;
    }
    else if (what == CONFIG_MAX_CONNECTIONS)
        _config[CONFIG_MAX_CONNECTIONS].config_values = (int*) 5;
    else if (what == CONFIG_MAX_AUTHENTICATION_TRIES)
        _config[CONFIG_MAX_AUTHENTICATION_TRIES].config_values = (int*) 3;
}

int set_property_value (void)
{
#ifdef CONFIGFILE
    //use it
#else
    char CONFIGFILE[BUFSIZ];
    strcpy (CONFIGFILE, getenv ("HOME") );
    strcat (CONFIGFILE, "/.jasm_config");
#endif

    FILE * fconfig;
    char get_buffer_from_file[BUFSIZ];
    int index;
    //long test_fsz;

    _config[CONFIG_MAX_CONNECTIONS].config_name = "MaxConnections";
    _config[CONFIG_MAX_AUTHENTICATION_TRIES].config_name = "MaxAuthTries";

    if ( (fconfig = fopen (CONFIGFILE, "r") ) == NULL)
    {
        set_default_property_value (CONFIG_ALL);
        return 1;
    }

    for (index = 0; index <= NCONFIG_PROPERTIES; index++)
    {
        char * get_val;
        if (fgets (get_buffer_from_file, BUFSIZ, fconfig) != NULL)
        {
#ifdef DEBUG
            char logstr_debug[BUFSIZ];
            sprintf (logstr_debug, "[JASM-DAEMON][DEBUG]Buffer: get_buffer_from_file=%s", get_buffer_from_file);
            log_string (logstr_debug);
#endif //DEBUG

            get_val = get_property_value (get_buffer_from_file);

#ifdef DEBUG
            sprintf (logstr_debug, "[JASM-DAEMON][DEBUG]Buffer: %s ,Prop: %s", get_buffer_from_file, get_val);
            log_string (logstr_debug);
#endif //DEBUG

            if (strcmp (get_buffer_from_file, "MaxAuthTries") == 0)
            {
                int auth_cast_integer_value = atoi (get_val);
                _config[CONFIG_MAX_AUTHENTICATION_TRIES].config_values = &auth_cast_integer_value;
            }

            if (strcmp (get_buffer_from_file, "MaxConnections") == 0)
            {
                int conn_cast_integer_value = atoi (get_val);
                _config[CONFIG_MAX_CONNECTIONS].config_values = &conn_cast_integer_value;
            }

        }
        else log_string ("[JASM-DAEMON][CONFIG]Configuration file END!");
    }

    fclose (fconfig);

    return 0;
}
