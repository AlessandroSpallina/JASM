#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include "cfgparser.h"
#include "logger.h"

// Yes, taken from stackoverflow
// http://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c
// we <3 you stackoverflow! :')
void delSpaces(char* _str)
{
    char *i = _str;
    char *j = _str;
    while(*j != 0) {
       *i = *j++;
       if(*i != ' ') {
         i++;
       }
    }
    *i = 0;
}

int get_values(struct configval *cfg)
{
    _Bool isset_maxconn=false;

#ifndef CONFIG_PATH
    char fname[256];
    strncpy(fname,getenv("HOME"),strlen(getenv("HOME")));
    strncat(fname,"/.jasm.conf",strlen("/.jasm.conf"));
#else
    char fname[256];
    strncpy(fname,CONFIG_PATH,strlen(CONFIG_PATH));
#endif //CONFIG_PATH

    FILE* fptr;
    
    if((fptr=fopen(fname,"r")) == NULL) {
        /* Default values */
        cfg->cfg_maxconn = __CONFIG_MAXCONN_DEFAULT;

        return -1;
    }
    
    while(1) {
        int i;
        char completeContent[256];
        
        bzero(completeContent,strlen(completeContent));
        
        if(fgets(completeContent,256,fptr) == NULL) {
            log_string("[JASM-DAEMON][CONFIG] Reached end of file.");
            break;
        }

        delSpaces(completeContent);

        char *token = strtok(completeContent,"=");
        if(strcmp(token,"MaxConn") == 0){
            int val = atoi(strtok(NULL,"="));
            if (val == 0) {
                val= __CONFIG_MAXCONN_DEFAULT;
            }

            cfg->cfg_maxconn = val;
            isset_maxconn=true;
        }
    }

    if(! isset_maxconn ) {
        cfg->cfg_maxconn = __CONFIG_MAXCONN_DEFAULT;
    }

    fclose(fptr);
    return 0;
}
