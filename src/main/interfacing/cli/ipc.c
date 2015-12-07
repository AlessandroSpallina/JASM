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

#include "ipc.h"


//Connects to the server and returns *** socket
int start_client(const char* srv_ip)
{
        int sockfd;
        int len;
        char get_msg_from_server[256];
        struct sockaddr_in address;
        int result;
        char get_my_pass[256];
        char passauth[10];

        sockfd=socket(AF_INET, SOCK_STREAM, 0); //return code checking

        address.sin_family=AF_INET;
        address.sin_addr.s_addr=inet_addr(srv_ip);
        address.sin_port=htons(SERVER_PORT);
        len=sizeof(address);

        result=connect(sockfd, (struct sockaddr *)&address, len);

        if(result < 0)
        {
                fprintf(stderr, "Unable to connect with server\n");
                exit(1);
        }

        if(read(sockfd,get_msg_from_server,sizeof(get_msg_from_server)) < 0)
                fprintf(stderr,"* Reading from socket error\n");

        printf("%s\n",get_msg_from_server);
        if(strcmp(get_msg_from_server, "auth-required") == 0)
        {

                printf("+-----------------------------------------------------------------------+\n");
                printf("* Authentication is required before accessing JASM Command Line Interface\n");
                printf("* Password: ");
                fgets(get_my_pass,BUFSIZ,stdin);

                if(write(sockfd, get_my_pass, 256) < 0)fprintf(stderr,"* Error writing pwd to server\n");
                if(read(sockfd,passauth,sizeof(passauth)) < 0)fprintf(stderr,"* Error reading server response\n");

                if(strcmp(passauth,"denied") == 0)
                {
                        printf("* Non-authorized access!!\n");
                        printf("* Exiting...\n");
                        close(sockfd);
                        exit(3);
                }
                else if(strcmp(passauth,"granted") == 0)
                {
                        printf("* Great                       *\n");
                        printf("* Authorized for this session *\n");
                }


        }
        else if(strcmp(get_msg_from_server, "auth-not-required") == 0)
        {
                char if_set_file[15];
                printf("* Authentication is not required for this session\n");

                if(read(sockfd,if_set_file,15)<0) fprintf(stderr,"* Error reading fexisting\n");
                printf("%s\n",if_set_file);

                if(strcmp(if_set_file,"check-pwd-file") == 0)
                {

                        char psw_to_use[256];

                        printf("* You have to set a password for JASM, in order to avoid intrusion\n");
                        printf("* This password will NOT be encrypted *\n");
                        printf("* Password to use[MAX: 256 chars]: ");

                        fgets(psw_to_use,BUFSIZ,stdin);
                        strcat(psw_to_use,"\0");
                        //printf("%s\n",psw_to_use);
                        if(write(sockfd,psw_to_use,sizeof(psw_to_use))<0)fprintf(stderr,"* Error sending pwd\n");
                }
                else if(strcmp(if_set_file,"nochk-pwdfile") == 0)
                {}
        }

        return sockfd;
}
