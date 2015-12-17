#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#define NCONFIG_PROPERTIES 2

extern char property_names[NCONFIG_PROPERTIES][BUFSIZ];
extern int property_values[NCONFIG_PROPERTIES];

extern int set_property_value(void);

#endif // CONFIGFILE_H
