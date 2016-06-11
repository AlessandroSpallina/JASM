#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "cfgparser.h"
#include "logger.h"

int get_values(struct configval *cfg)
{
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
        fclose(fptr);
        return -1;
    }
    
    while(1) {
        char log_value[256], completeContent[256];
        
        bzero(completeContent,strlen(completeContent));
        
        if(fgets(completeContent,256,fptr) == NULL) {
            break;
        }
        
        char *token = strtok(completeContent," = ");
        if(strcmp(token,"MaxAuthTries") == 0){
            sprintf(log_value,"--> MaxAuthTries %s",token);
            log_string(log_value);
            break;
        }
        
    }
    
    fclose(fptr);
    return 0;
}