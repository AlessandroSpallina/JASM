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
        char answer[10];
        
        sockfd=socket(AF_INET, SOCK_STREAM, 0);

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

        if(read(sockfd,get_msg_from_server,256) < 0)
         printf("* Read error\n");

        if(strcmp(get_msg_from_server, "auth-required") == 0)
        {
          printf("+-----------------------------------------------------------------------+\n");
          printf("* Authentication is required before accessing JASM Command Line Interface\n");
          printf("* Password: ");
          //fgets(get_my_pass,sizeof(get_my_pass),stdin);
          scanf("%s",&get_my_pass); //temporary
          
          if(write(sockfd,get_my_pass,256) < 0)
           printf("* Error on write\n");
           
          if(read(sockfd,answer,sizeof(answer)) < 0)
           printf("* Error on read\n");
   
          if(strcmp(answer,"denied") == 0)
          {
			  printf("* Non-authorized access!!\n");
			  printf("* Exiting...\n");
			  close(sockfd);
			  exit(3);
		  }
		  else if(strcmp(answer,"granted") == 0)
		  {
			  printf("* Great                       *\n");
			  printf("* Authorized for this session *\n");
          }
	  }
      else if(strcmp(get_msg_from_server, "auth-not-required") == 0)
        printf("* Authentication is not required for this session\n");
          
        return sockfd;
}
