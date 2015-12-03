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

void print_welcome(const char* usern, int sockfd)
{
        printf("JASM Command Line Interface\nSession started: %s\nSocket fd: %d\nUser: %s\nBasic Commands:\nhelp : get help\
  \nquit : exits cli\nhalt : halt jasm\n\n",getTime(),sockfd,usern);
}

void secureJasmCommunication(char buffer[BUFSIZ], int fd)
{

        if(strcmp("quit", buffer)==0) {
                close(fd);
                printf("Bye!\n");
                exit(0);
        } else {

                if(strcmp("help", buffer)==0) {
                        int ngetter=0;

                        printf("Sending [%s]\n\n", buffer);
                        if(write(fd, "help\0", 5)<0)
                                perror("write on fd FAIL");

                        //riceve i getter
                        if(read(fd, &ngetter, sizeof(ngetter))<0)
                                perror("read on fd FAIL");

                        printf("\n**JASM Help page**\n");
                        printf("* Getters *\n");
                        for(int i=0; i<ngetter; i++) {
                                memset(buffer, 0, BUFSIZ);
                                if(read(fd, (void *)buffer, sizeof(buffer))<0)
                                        perror("read on fd FAIL");
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
                        int n;

                        printf("sending [%s - %d byte]\n", buffer, (int) write(fd, (void *)buffer, strlen(buffer)+1));
                        memset(buffer, 0, BUFSIZ);
                        n=read(fd, (void *)buffer, sizeof(buffer));
                        printf("receiving [%s - %d byte]\n", buffer, n);
                        return;
                }

        }

}

/*
   void clean_socket(int fd)
   {
   char tmp[BUFSIZ];

   read(fd, tmp, sizeof(tmp));
   printf("clean: %s\n", tmp);
   }
 */

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
                                exit(3);
                        }
                }
        }
        //else if ...
}

int main(int argc, char *argv[])
{
        int fd;
        char buf[BUFSIZ]="none";
        char *username=getenv("USER");

        server_ip = "127.0.0.1";

        parse_options(argc, argv);

        fd=start_client(server_ip);

        print_welcome(username, fd);

        signal_catcher();
        //clean_socket(fd);

        while(1) {
                printf("-[%s]-> ", username);
                scanf("%s", buf);
                secureJasmCommunication(buf, fd);
                printf("\n");
        }
}
