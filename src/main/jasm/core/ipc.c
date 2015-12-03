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
#include <crypt.h>

#include "ipc.h"
#include "miscellaneous.h"
#include "getter.h"


static void excecute_command(int fd, char *command)
{
        /*
         *  if get* -> module get
         *  if start* -> module of  modules
         */

        // write on fd a list of commands
        if(strcmp("help", command)==0) {
                getGetter(fd);
                //getModules
                //getOther

                log_string("[CMD] help exec");
                return;
        }

        // ************************** getter ***************************************
        if(strncmp("get", command, 3)==0) { //if recv get command
                int i;

                strcpy(command, &command[3]);

                for(i=0; i<NGETTER; i++) {
                        if(strcmp(getterName[i], command)==0) { //if getter exists
                                log_string("Getter found :)");
                                getterFunction[i](fd);
                                return;
                        }
                }

                log_error("Getter NOT found :(");
                write(fd, "null\0", 5);
                return;
        }

        // ************************** starter **************************************
        if(strncmp("start", command, 5)==0) { //recieved start mod

                log_error("Start NOT found :(");
                write(fd, "null\0", 5);
                return;
        }

        // ************************** miscellaneous ********************************
        if(strcmp("halt", command)==0) { //turn off jasm
                log_string("[CMD] halt exec");
                write(fd, "Killing JASM...\0", 18);
                exit(0);
        }

        log_error("[CMD] Command not found!");
        write(fd, "NotFound\0", 9);
}

void start_server()
{
	#ifdef PASSWD_ENC_FILE
	//use it
	#else
	char PASSWD_ENC_FILE[256];
	strcpy(PASSWD_ENC_FILE, getenv("HOME"));
	strcat(PASSWD_ENC_FILE, "/.jpwd");
	#endif

        int server_sockfd, client_sockfd;
        int server_len;
        socklen_t client_len;
        struct sockaddr_in server_address;
        struct sockaddr_in client_address;
        int result;
        char client_ipaddr[30]; // testing purposes

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

        log_string("[JASM-DAEMON]Server started");

        while(1) {
                char buf[BUFSIZ];
                char received[BUFSIZ];
                int fd;
                int nread;

                testfds=readfds;

                result=select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);

                if(result<1) {
                        log_error("[JASM-DAEMON]Server failed");
                        exit(1);
                }

                for(fd=0; fd<FD_SETSIZE; fd++) {
                        if(FD_ISSET(fd, &testfds)) {
                                if(fd==server_sockfd) {
                                        client_len=sizeof(client_address);
                                        client_sockfd=accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
                                        FD_SET(client_sockfd, &readfds);
                                        sprintf(client_ipaddr, "%d.%d.%d.%d", client_address.sin_addr.s_addr&0xFF,(client_address.sin_addr.s_addr&0xFF00)>>8, (client_address.sin_addr.s_addr&0xFF0000)>>16, (client_address.sin_addr.s_addr&0xFF000000)>>24);
										if(login_required(client_ipaddr) == 1)
										{
                                          char getpasswd[256];
                                          char auth[256]="auth-required";
                                          char granted[7]="granted";
                                          char denied[6]="denied";
                                          //char chkpwd[15]="check-pwd-file";
                                          //checks that password file exists!
                                          /**PRIORITY FOR  check_passwd_file() **/
										  //check_passwd_file(chkpwd,15,client_sockfd);
           								  log_string("[CLIENT-AUTH]Authentication required! ...");
                                          if(write(client_sockfd,auth,256) < 0) log_error("[write()][auth] Error\n");
                                          if(read(client_sockfd,getpasswd,256) < 0) log_error("[read()][getpasswd] Error\n");
                                          //log_string(getpasswd);
                                          //check here
                                          if(strcmp(getpasswd,"jasmtest") == 0)
                                          {
											log_string("[PWD][OK]Password accepted!\n");
											log_string("[PWD][OK]Authorized!\n");
											if(write(client_sockfd,granted,7) < 0)
											  log_error("[core/ipc.c][start_server()][getpasswd][write()] ERROR while sending granted\n");
								          }
                                          else if(strcmp(getpasswd,"jasmtest") != 0)
                                          {
											 log_error("[PWD][DEN]Wrong password!\n");
											 log_error("[PWD][DEN]Closing connection...\n");
											 if (write(client_sockfd,denied,6) < 0)
											   log_error("[core/ipc.c][start_server()][getpasswd][write()] ERROR while sending denied\n");
										  }
										}
                                        else
                                        {
										  /**WIP**/
										  char chkpwd[14]="check-pwd-file";
										  char nochkpwd[13]="nochk-pwdfile";
										  char getpwd[256];

									      char not_required[18]="auth-not-required";
                                          if(write(client_sockfd,not_required,18) < 0) log_error("[write()] Error\n");
                                          log_string("[CLIENT-AUTH]Authentication NOT required!\n");
                                          if(write(client_sockfd,chkpwd,14) < 0) log_error("[write()] Error\n");
                                          //                     ^-> replace with chkpwd
                                          //check_passwd_file(PASSWD_ENC_FILE,"something");
                                        }

                                        sprintf(buf, "[CLIENT-CONNECT] sockfd: %d, IP Address: %d.%d.%d.%d\
                                        ", client_sockfd, client_address.sin_addr.s_addr&0xFF,(client_address.sin_addr.s_addr&0xFF00)>>8, (client_address.sin_addr.s_addr&0xFF0000)>>16, (client_address.sin_addr.s_addr&0xFF000000)>>24);
                                        //Using the client struct
                                        log_string(buf);
                                } else {
                                        ioctl(fd, FIONREAD, &nread);

                                        if(nread==0) {
                                                close(fd);
                                                FD_CLR(fd, &readfds);
                                                sprintf(buf, "[CLIENT-DISCONNECT] sockfd: %d, IP Address: %d.%d.%d.%d \
                                                ", fd, client_address.sin_addr.s_addr&0xFF, (client_address.sin_addr.s_addr&0xFF00)>>8, (client_address.sin_addr.s_addr&0xFF0000)>>16, (client_address.sin_addr.s_addr&0xFF000000)>>24);
                                                //Using the client struct
                                                log_string(buf);
                                        } else {
                                                read(fd, &received, BUFSIZ);
                                                sprintf(buf, "[CMD-GET] Got command from %d: <%s>", fd, received);
                                                log_string(buf);
                                                excecute_command(fd, received);
                                        }
                                }
                        }
                }
        }
}
