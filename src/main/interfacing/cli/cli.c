/****************************************************************************
* Copyright © 2015 Alessandro Spallina
* email: alessandrospallina1@gmail.com
* github: https://github.com/AlessandroSpallina
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ipc.h"
#include "miscellaneous.h"
#include "signals.h"

char* server_ip;

void print_welcome(const char* usern, int sockfd,const char* releasetime, const char* debugrel)
{
  #ifdef CHECK_ACCESS_FILE
  //use it
  #else
  char CHECK_ACCESS_FILE[256];
  strcpy(CHECK_ACCESS_FILE,getenv("HOME"));
  strcat(CHECK_ACCESS_FILE,"/.jpwdchk");
  #endif // CHECK_ACCESS_FILE

        if(check_if_file_exists(CHECK_ACCESS_FILE)==1)
        {
            char msg_from_file[BUFSIZ];
            FILE * fn;
            if((fn=fopen(CHECK_ACCESS_FILE,"r"))!=NULL)
            {
                fgets(msg_from_file,BUFSIZ,fn);
                if(strcmp(msg_from_file,"true")==0)
                {
                    printf("*IMPORTANT NOTICE*\n*JASM detected that someone tried to access this server more than 3 times!\n*Check server log!\n");
                    fclose(fn);
                    if((fn=fopen(CHECK_ACCESS_FILE,"w"))==NULL){}
                    fprintf(fn,"false");
                    fclose(fn);
                }
                else if(strcmp(msg_from_file,"false")==0)
                    fclose(fn);
            }
        }
        else
        {}

        printf("%s\n",debugrel);
        printf("JASM Command Line Interface\nBuild date: %s\nSession started: %s\nSocket fd: %d\nUser: %s\nBasic Commands:\nhelp : get help\
  \nquit : exits cli\nhalt : halt jasm\n\n",releasetime,getTime(),sockfd,usern);
}

void secureJasmCommunication(char * buffer, int fd)
{
                int n;

                if(strcmp("help", buffer)==0) {
                        int ngetter=0;

                        printf("Sending [%s]\n\n", buffer);
                        if(write(fd, "help", strlen("help"))<0)
                        {
                                perror("write on fd FAIL");
                        }

                        //gets getter list
                        if((n=read(fd, &ngetter, sizeof(ngetter)))<0)
                        {
                                perror("read on fd FAIL");
                        }
                        else if(n==0)
                        {
                            #ifdef DEBUG
                            printf("[DEBUG]Errno: %s",strerror(errno));
                            #endif // DEBUG

                            printf("* Server disconnected\n");
                            exit(SERVER_DISCONNECTED);
                        }

                        printf("\n**JASM Help page**\n");
                        printf("* Getters *\n");
                        for(int i=0; i<ngetter; i++) {
                                memset(buffer, 0, BUFSIZ);
                                if((n=read(fd, (void*)buffer, sizeof(buffer)))<0)
                                {
                                        perror("read on fd FAIL");
                                        #ifdef DEBUG
                                        fprintf(stderr,"[DEBUG] Errno result: %s\n",strerror(errno));
                                        #endif
                                }
                                else if(n == 0)
                                {
                                    printf("* Server disconnected\n");
                                    exit(SERVER_DISCONNECTED);
                                }
                                printf("%d) %s\n", i, buffer);
                        }
                        //riceve gli starter dei moduli
                        //riceve altro
                        return;
                }

                if(strncmp("start", buffer, 5)==0) {
                        //starting thread-module -> opening new dedicated socket
                        //so we can get different output from jasm thanks to fd.

                        /*
                         *	new socket
                         *	write()
                         *	read()
                         */
                        return;

                } else {

                        printf("sending [%s - %d byte]\n", buffer, (int) write(fd, (void *)buffer, strlen(buffer)+1));
                        memset(buffer, 0, BUFSIZ);
                        n=read(fd, (void *)buffer, sizeof(buffer));
                        if(n < 0)
                        {
                          perror("* Error on read()");
                          #ifdef DEBUG
                          printf("[DEBUG] Errno result: %s\n",strerror(errno));
                          #endif
                        }
                        else if(n == 0)
                        {
                            #ifdef DEBUG
                            printf("[DEBUG] Errno result: %s\n",strerror(errno));
                            #endif // DEBUG
                            printf("* Server disconnected\n");
                            exit(SERVER_DISCONNECTED);
                        }

                        printf("receiving [%s - %d byte]\n", buffer, n);
                        return;
                }

}

void parse_options(int argc, char *argv[])
{
        if(argc > 1 && argc <= 3)
        {
                if(strcmp(argv[1],"--connect-server") == 0 )
                {
                        if(argc == 3)
                        {
                                server_ip = argv[2];
                                printf("* Connecting to: %s ...\n",server_ip);
                        }
                        else if(argc < 3)
                        {
                                printf("* You must specify an IP address!\n");
                                exit(ARG_SPECIFY_IPADDR);
                        }
                }
        }
        //else if ...
}

int main(int argc, char *argv[])
{
        check_debug();
        check_release();

        char buf[BUFSIZ];
        int fd;
        char *username=getenv("USER");

        server_ip = "127.0.0.1";

        parse_options(argc, argv);

        fd=start_client(server_ip);

        print_welcome(username, fd, buildtime, debugstr);

        signal_catcher();


        while(1) {
                printf("-[%s]-> ", username);
                scanf("%s", buf);
                if(strcmp(buf, "quit")==0) {
                	close(fd);
                	printf("Bye!\n");
                	exit(_EXIT_SUCCESS);
                } else {
                	secureJasmCommunication(buf, fd);
                }
                printf("\n");
        }
}
