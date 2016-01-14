#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#define NCONFIG_PROPERTIES 2
#define NCONFIG_PROPERTIES_COUNTER NCONFIG_PROPERTIES-1

#define CONFIG_MAX_AUTHENTICATION_TRIES 1
#define CONFIG_MAX_CONNECTIONS 0
#define CONFIG_ALL -1

extern struct config
{
    char config_name[BUFSIZ];
    void * config_values;
} _config[NCONFIG_PROPERTIES];

extern int set_property_value (void);

#endif // CONFIGFILE_H
