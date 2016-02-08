#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "miscellaneous.h"
#include "configfile.h"
#include "logger.h"

struct config _config[NCONFIG_PROPERTIES]; //access data through this struct

char * get_property_value (char * __prop)
{
        if (__prop == NULL)
        {
#ifdef DEBUG
                char logstr_debug[BUFSIZ];
                sprintf (logstr_debug, "[JASM-DAEMON][DEBUG]__prop string is NULL");
                log_string (logstr_debug);
#endif //DEBUG
       // make sure the calling programs checks this value
                return NULL;
        }
        strtok (__prop, "=");

        return strtok(NULL,"=");
}

void set_default_property_value (int what)
{
        int predef_values[NCONFIG_PROPERTIES] = {
                3,
                5
        };
        if (what == CONFIG_ALL)
        {
                for(int prop_counter=0; prop_counter<=NCONFIG_PROPERTIES_COUNTER; prop_counter++)
                        _config[prop_counter].config_values = &predef_values[prop_counter];
        }
        else if (what == CONFIG_MAX_CONNECTIONS)
                _config[CONFIG_MAX_CONNECTIONS].config_values = &predef_values[CONFIG_MAX_CONNECTIONS];
        else if (what == CONFIG_MAX_AUTHENTICATION_TRIES)
                _config[CONFIG_MAX_AUTHENTICATION_TRIES].config_values = &predef_values[CONFIG_MAX_AUTHENTICATION_TRIES];
        else
        {
#ifdef DEBUG
                char logstr_debug[BUFSIZ];
                sprintf (logstr_debug, "[JASM-DAEMON][DEBUG]what value is not one of the predefined values, setting CONFIG_ALL");
                log_string (logstr_debug);
#endif //DEBUG
       // this is the equivalent of what == CONFIG_ALL
                for(int prop_counter=0; prop_counter<=NCONFIG_PROPERTIES_COUNTER; prop_counter++)
                        _config[prop_counter].config_values = &predef_values[prop_counter];
        }
}

int set_property_value (void)
{
#ifdef CONFIGFILE
        //use it
#else
        char CONFIGFILE[BUFSIZ];
        // use the bounded version now
        strcpy (CONFIGFILE, getenv ("HOME"));
        strcat (CONFIGFILE, "/.jasm_config");
#endif

        FILE *fconfig;
        char get_buffer_from_file[BUFSIZ];

#ifdef DEBUG
        char logstr_debug[BUFSIZ];
#endif

        strcpy (_config[CONFIG_MAX_CONNECTIONS].config_name,"MaxConnections");
        strcpy (_config[CONFIG_MAX_AUTHENTICATION_TRIES].config_name,"MaxAuthTries");

        if ( (fconfig = fopen (CONFIGFILE, "r") ) == NULL)
        {
                set_default_property_value (CONFIG_ALL);
                return 1;
        }

        char *get_val = NULL;
        while (fgets (get_buffer_from_file, BUFSIZ, fconfig) != NULL)
        {
#ifdef DEBUG
                sprintf (logstr_debug, "[JASM-DAEMON][DEBUG]Buffer: get_buffer_from_file=%s", get_buffer_from_file);
                log_string (logstr_debug);
#endif //DEBUG

                if (get_buffer_from_file[0] == '#') {
#ifdef DEBUG
                        log_string("[JASM-DAEMON][DEBUG][CONFIG]Comment detected, ignoring...");
#endif //DEBUG
                        continue;
                }

                get_val = get_property_value (get_buffer_from_file);

                if (get_val == NULL)
                {
#ifdef DEBUG
                        sprintf (logstr_debug, "[JASM-DAEMON][DEBUG]get_val is NULL. config set to CONFIG_ALL");
                        log_string (logstr_debug);
#endif //DEBUG
                        log_string("[JASM-DAEMON][WARN]Check your config file! Something is wrong!! Setting default values...");
                        set_default_property_value (CONFIG_ALL);
                        fclose (fconfig);
                        return -1;
                }

#ifdef DEBUG
                sprintf (logstr_debug, "[JASM-DAEMON][DEBUG]Buffer: %s ,Prop: %s", get_buffer_from_file, get_val);
                log_string (logstr_debug);
#endif //DEBUG

                // add bounded check for string comparison, to prevent buffer overflow
                if (strncmp (get_buffer_from_file, "MaxAuthTries", strlen (get_buffer_from_file) ) == 0)
                {
                        // make a promise to change atoi to strtol to ease the error checking phase
                        int auth_cast_integer_value = atoi (get_val);
                        _config[CONFIG_MAX_AUTHENTICATION_TRIES].config_values = &auth_cast_integer_value;
                }

                // add bounded check for string comparison, to prevent buffer overflow
                if (strncmp (get_buffer_from_file, "MaxConnections", strlen (get_buffer_from_file) ) == 0)
                {
                        int conn_cast_integer_value = atoi (get_val);
                        _config[CONFIG_MAX_CONNECTIONS].config_values = &conn_cast_integer_value;
                }
        }
        fclose (fconfig);

        //test entries
        for (int ind=0; ind<=NCONFIG_PROPERTIES_COUNTER; ind++) {
#ifdef DEBUG
                sprintf(logstr_debug,"[JASM-DAEMON][CONFIG][DEBUG] Cycle Index: %d",ind);
                log_string(logstr_debug);
#endif //DEBUG
                if (_config[ind].config_values == NULL) {
                        set_default_property_value(ind);
#ifdef DEBUG
                        sprintf(logstr_debug,"[JASM-DAEMON][CONFIG][DEBUG] Predef - Key: %s, Value: %d",_config[ind].config_name,*(int*)_config[ind].config_values);
                        log_string(logstr_debug);
#endif //DEBUG
                }
        }
        return 0;
}
