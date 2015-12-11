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

//NOTE:Error codes defined @ miscellaneous.h

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
        if(sockfd < 0)
        {
          perror("* Error while creating new socket\n* Exiting...\n");
          #ifdef DEBUG
          fprintf(stderr,"[DEBUG] Errno result: %s\n",strerror(errno));
          #endif
          exit(SOCKET_CREATION_FAILED);
        }

        address.sin_family=AF_INET;
        address.sin_addr.s_addr=inet_addr(srv_ip);
        address.sin_port=htons(SERVER_PORT);
        len=sizeof(address);

        result=connect(sockfd, (struct sockaddr *)&address, len);

        if(result < 0)
        {
                perror("Unable to connect with server\n");
                #ifdef DEBUG
                printf("[DEBUG] Errno result: %s\n",strerror(errno));
                #endif
                exit(SOCKET_CONNECTION_FAILED);
        }

        if(read(sockfd,get_msg_from_server,sizeof(get_msg_from_server)) < 0)
        {
          #ifdef DEBUG
          printf("[DEBUG] Errno result: %s\n",strerror(errno));
          #endif
                perror("* Reading from socket error\n");
        }

        #ifdef DEBUG
        printf("[DEBUG] Auth response: %s\n",get_msg_from_server);
        #endif

        if(strcmp(get_msg_from_server, "auth-required") == 0)
        {
                printf("+-----------------------------------------------------------------------+\n");
                printf("* Authentication is required before accessing JASM Command Line Interface\n");
                printf("* Password: ");
                fgets(get_my_pass,BUFSIZ,stdin);

                #ifdef DEBUG
                printf("[DEBUG] Password: %s\n",get_my_pass);
                #endif

                if(write(sockfd, get_my_pass, 256) < 0)
                {
                  perror("Error sending password!");
                  #ifdef DEBUG
                  printf("[DEBUG] Errno result: %s\n",strerror(errno));
                  #endif
                }

                if(read(sockfd,passauth,sizeof(passauth)) < 0)
                {
                  perror("* Error reading server response\n");
                  #ifdef DEBUG
                  printf("[DEBUG] Errno result: %s\n",strerror(errno));
                  #endif
                }

                if(strcmp(passauth,"denied") == 0)
                {
                        char if_retry[BUFSIZ];

                        for(int i=1;i<=4;i++)
                        {

                            if(read(sockfd,if_retry,sizeof(if_retry)) < 0)
                                perror("* Error while getting response...\n");

                            #ifdef DEBUG
                            printf("[DEBUG] Response: %s\n",if_retry);
                            #endif // DEBUG

                            if(strcmp(if_retry,"ry")==0 ||
                               strcmp(if_retry,"retry")== 0)
                            {
                                if(i==4)
                                {

                                    printf("* Too much attempts!\n");
                                    printf("* Closing connection...\n");
                                    printf("* To prevent intrusion!\n");
                                    close(sockfd);
                                    exit(LOGIN_TOO_MUCH_ATTEMPTS);
                                 }

                                printf("* Attempt: %d\n",i);
                                printf("* Retry\n");
                                printf("* Password: ");
                                fgets(get_my_pass,BUFSIZ,stdin);
                                if(write(sockfd,get_my_pass,sizeof(get_my_pass)) < 0)
                                    perror("* Error while sending pswd");
                            }
                            else if(strcmp(if_retry,"authorized")==0)
                            {
                                printf("* Authorized [attempt: %d]\n",i);
                                break;
                            }
                        }
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

                if(read(sockfd,if_set_file,15)<0)
                {
                  perror("* Error reading fexisting\n");
                  #ifdef DEBUG
                  printf("[DEBUG] Errno result: %s\n",strerror(errno));
                  #endif
                }

                #ifdef DEBUG
                printf("[DEBUG] File response: %s\n",if_set_file);
                #endif

                if(strcmp(if_set_file,"check-pwd-file") == 0)
                {

                        char psw_to_use[256];

                        printf("* You have to set a password for JASM, in order to avoid intrusion\n");
                        printf("* This password will NOT be encrypted *\n");
                        printf("* Password to use[MAX: 256 chars]: ");

                        fgets(psw_to_use,BUFSIZ,stdin);
                        strcat(psw_to_use,"\0");

                        #ifdef DEBUG
                        printf("[DEBUG] Password: %s\n",psw_to_use);
                        #endif

                        if(write(sockfd,psw_to_use,sizeof(psw_to_use))<0)
                        {
                          perror("* Error sending pwd\n");
                          #ifdef DEBUG
                          fprintf(stderr,"[DEBUG] Errno result: %s\n",strerror(errno));
                          #endif
                        }
                }
                else if(strcmp(if_set_file,"nochk-pwdfile") == 0)
                {}
        }

        return sockfd;
}
