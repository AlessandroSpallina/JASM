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
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "ipc.h"
#include "miscellaneous.h"
#include "getter.h"


static void excecute_command(int fd, char *command)
{
  /*
   *  if get* -> module get
   *  if start* -> module of  modules
   */

   // ************************** getter ***************************************
   if(strncmp("get", command, 3)==0) {  //if recv get command
     int i;
     //char buf[BUFSIZ];

     strcpy(command, &command[3]);

     for(i=0; i<NGETTER; i++) {
       if(strcmp(getterName[i], command)==0) { //if getter exists
         log_string("getter found :)");
         getterFunction[i](fd);
         return;
       }
     }

     log_error("getter NOT found :(");
     write(fd, "null\0", 4);
     return;
   }

   // ************************** starter **************************************
   if(strncmp("start", command, 5)==0) {  //recieved start mod



	 log_error("starter NOT found :(");
	 write(fd, "null\0", 4);
     return;
   }


   // ************************** miscellaneous ********************************
   if(strcmp("halt", command)==0) { //turn off jasm
		log_string("# halt and catch fire, done");
		write(fd, "halt\0", 4);
		exit(0);
   }



  /*if(strcmp("getVersion", command)==0) {
    write(fd, (void *)VERSION, sizeof(VERSION));
    log_string("server reply <version> with success");
    return;
  }*/
    log_error("request not found");
    write(fd, "null\0", 4);

}

void start_server()
{
int server_sockfd, client_sockfd;
int server_len;
socklen_t client_len;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
int result;
fd_set readfds, testfds;

  server_sockfd=socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family=AF_INET;
  server_address.sin_addr.s_addr=htonl(INADDR_ANY);
  server_address.sin_port=htons(SERVER_PORT);
  server_len=sizeof(server_address);

  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

  listen(server_sockfd, 5);

  FD_ZERO(&readfds);
  FD_SET(server_sockfd, &readfds);

  log_string("server started");

  while(1) {
    char buf[BUFSIZ];
    char received[BUFSIZ];
    int fd;
    int nread;

    testfds=readfds;

    result=select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);

    if(result<1) {
      log_error("server fail");
      exit(1);
    }

    for(fd=0; fd<FD_SETSIZE; fd++) {
      if(FD_ISSET(fd, &testfds)) {
        if(fd==server_sockfd) {
          client_len=sizeof(client_address);
          client_sockfd=accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
          FD_SET(client_sockfd, &readfds);
          sprintf(buf, "adding client on fd %d", client_sockfd);
          log_string(buf);
        } else {
          ioctl(fd, FIONREAD, &nread);

          if(nread==0) {
            close(fd);
            FD_CLR(fd, &readfds);
            sprintf(buf, "removing client on fd %d", fd);
            log_string(buf);
          } else {
            read(fd, &received, BUFSIZ);
            sprintf(buf, "received from fd %d command <%s>", fd, received);
            log_string(buf);
            excecute_command(fd, received);
          }
        }
      }
    }
  }
}
