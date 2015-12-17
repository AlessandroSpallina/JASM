#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "configfile.h"

char property_names[NCONFIG_PROPERTIES][BUFSIZ]={
  "MaxAuthTries",
  "MaxConnections"
};

int property_values[NCONFIG_PROPERTIES]={
  0,
  0
};

char * get_property_value(char * __prop)
{
        char * propval = strtok(__prop,"=");
        return strtok(NULL,"=");
}

int set_property_value(void)
{
        #ifdef CONFIGFILE
        //use it
        #else
        char CONFIGFILE[BUFSIZ];
        strcpy(CONFIGFILE,getenv("HOME"));
        strcat(CONFIGFILE,"/.jasm_config");
        #endif

        FILE * fconfig;
        char get_buffer_from_file[BUFSIZ];
        int index=0;

        if((fconfig=fopen(CONFIGFILE,"r"))==NULL){
          property_values[0]=3;
          property_values[1]=3;
          return 1;
        }

        while(fgets(get_buffer_from_file,BUFSIZ,fconfig) != NULL){
          index++;
          char * get_val;
          get_val = get_property_value(get_buffer_from_file);
          property_values[index] = atoi(get_buffer_from_file);
        }

        fclose(fconfig);

        return 0;
}
