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
#include <syslog.h>

#include "ipc.h"
#include "miscellaneous.h"
#include "getter.h"

char errlog[BUFSIZ];

//NOTE: Error codes defined @ miscellaneous.h

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
                openlog("JASM",LOG_PID,LOG_DAEMON);
                syslog(LOG_INFO,"exiting as requested from client...");
                closelog();
                exit(_EXIT_SUCCESS);
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
        char client_ipaddr[30];

        fd_set readfds, testfds;

        client_sockfd=0;

        server_sockfd=socket(AF_INET, SOCK_STREAM, 0); //fix
        if(server_sockfd < 0)
        {
          log_error("[JASM-DAEMON][socket()]Failed to create new socket! Exiting...\n");
          openlog("JASM",LOG_PID,LOG_DAEMON);
          syslog(LOG_ERR,"socket creation failed!! Exiting!");
          closelog();
          exit(SOCKET_CREATION_FAILED);
        }
        server_address.sin_family=AF_INET;
        server_address.sin_addr.s_addr=htonl(INADDR_ANY);
        server_address.sin_port=htons(SERVER_PORT);
        server_len=sizeof(server_address);

        if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0)
        {
            char *error_dyn="null";
            sprintf(error_dyn,"[JASM-DAEMON][bind()]Error: %s\n",strerror(errno));
            log_error("[JASM-DAEMON][bind()]Failed to bind socket!\n");
            log_error(error_dyn);
            log_error("[JASM-DAEMON] Exiting !\n");
            openlog("JASM",LOG_PID,LOG_DAEMON);
            syslog(LOG_ERR,"socket not correctly binded... exiting!");
            closelog();
            exit(SOCKET_BINDING_FAILED);
        }

        if(listen(server_sockfd, 5) < 0)
        {
            log_error("[JASM-DAEMON][listen()]Failed to put socket in listening mode! \n");
            openlog("JASM",LOG_PID,LOG_DAEMON);
            syslog(LOG_ERR,"FATAL! socket was not put to listening mode! Exiting...");
            closelog();
            exit(SOCKET_LISTENING_CONNECTION_FAILED);
        }

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
                        sprintf(errlog,"[JASM-DAEMON]Error: %s\n",strerror(errno));
                        log_error("[JASM-DAEMON][select()]Server failed");
                        log_error(errlog);
                        exit(SOCKET_SELECT_FAILED);
                }

                for(fd=0; fd<FD_SETSIZE; fd++) {
                        if(FD_ISSET(fd, &testfds)) {
                                if(fd==server_sockfd) {
                                        client_len=sizeof(client_address);
                                        client_sockfd=accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
                                        if(client_sockfd < 0)
                                        {
                                          log_error("[JASM-DAEMON][accept()]Failed to accept socket connection!\n");
                                          openlog("JASM",LOG_PID,LOG_DAEMON);
                                          syslog(LOG_ERR,"FATAL! Failed to accept client incoming connection! Exiting...");
                                          closelog();
                                          exit(SOCKET_CLIENT_CONNECTION_FAILED);
                                        }

                                        FD_SET(client_sockfd, &readfds);
                                        sprintf(client_ipaddr, "%d.%d.%d.%d", client_address.sin_addr.s_addr&0xFF,(client_address.sin_addr.s_addr&0xFF00)>>8, (client_address.sin_addr.s_addr&0xFF0000)>>16, (client_address.sin_addr.s_addr&0xFF000000)>>24);
                                        if(login_required(client_ipaddr) == 1)
                                        {
                                                int rpwd;
                                                char getpasswd[256];
                                                char auth[14]="auth-required\0";
                                                char granted[8]="granted\0";
                                                char denied[7]="denied\0";
                                                char passwd_from_client[256];

                                                FILE* source_passwd;

                                                log_string("[CLIENT-AUTH]Authentication required! ...");
                                                if(write(client_sockfd,auth,sizeof(auth)) < 0)
                                                        log_error("[write()][auth] Error\n");

                                                if((rpwd = read(client_sockfd,getpasswd,sizeof(getpasswd))) < 0)
                                                    log_error("[read()][getpasswd] Error\n");

                                                if((source_passwd=fopen(PASSWD_ENC_FILE,"r")) != NULL)
                                                {
                                                        fgets(passwd_from_client,BUFSIZ,source_passwd);
                                                        fclose(source_passwd);
                                                }
                                                else
                                                {
                                                        log_error("[JASM-DAEMON][FILE]Password file not found!");
                                                        log_error("[SECURITY]JASM is going to be killed to avoid intrusion");
                                                        exit(NOFILE_ERROR);
                                                }

                                                if(strcmp(getpasswd,passwd_from_client) == 0)
                                                {
                                                        log_string("[PWD][OK]Password accepted!\n");
                                                        log_string("[PWD][OK]Authorized!\n");
                                                        if(write(client_sockfd,granted,8) < 0)
                                                                log_error("[core/ipc.c][start_server()][getpasswd][write()] ERROR while sending granted\n");
                                                }
                                                else if(strcmp(getpasswd,passwd_from_client) != 0)
                                                {

                                                        log_error("[PWD][DEN]Wrong password!\n");
                                                        if(write(client_sockfd,denied,7)<0)
                                                            log_error("[JASM-DAEMON][write()] Error!\n");
                                                        if(write(client_sockfd,"retry\0",6) < 0)
                                                               log_error("[JASM-DAEMON][write()] Error\n");

                                                        for(int i=1;i<=4;i++)
                                                        {

                                                            char passwd[256];

                                                            int nbs = read(client_sockfd,passwd,sizeof(passwd));
                                                            if(nbs==0) break;
                                                            else if(nbs < 0)
                                                                log_error("[JASM-DAEMON][read()] Error\n");

                                                            if(strcmp(passwd,passwd_from_client) == 0)
                                                            {
                                                                if(write(client_sockfd,"authorized\0",11))
                                                                    log_error("[JASM-DAEMON][write()] Error\n");
                                                                break;
                                                            }
                                                            else if(strcmp(passwd,passwd_from_client) != 0)
                                                            {
                                                                if(write(client_sockfd,"retry\0",6))
                                                                    log_error("[JASM-DAEMON][write()] Error\n");

                                                                 if(i==3) shutdown(client_sockfd,2);
                                                            }


                                                        }
                                                }
                                        }
                                        else
                                        {
                                                /**WIP**/
                                                char chkpwd[15]="check-pwd-file\0";
                                                char nochkpwd[14]="nochk-pwdfile\0";
                                                int chkfile;

                                                char not_required[18]="auth-not-required\0";
                                                if(write(client_sockfd,not_required,18) < 0) log_error("[not_required][write()] Error\n");
                                                log_string("[CLIENT-AUTH]Authentication NOT required!\n");
                                                chkfile=check_passwd_file(PASSWD_ENC_FILE);

                                                if(chkfile == 0)
                                                {
                                                        if(write(client_sockfd,nochkpwd,14) < 0)
                                                                log_error("[chkfile][retval0][write()] error\n");
                                                }
                                                else if(chkfile == 1)
                                                {
                                                        FILE *pswfp;
                                                        char buf_in_passwd[256];

                                                        if(write(client_sockfd,chkpwd,15) < 0) log_error("write() error\n");
                                                        if(read(client_sockfd,buf_in_passwd,sizeof(buf_in_passwd)) < 0) //sizeof() may be replaced
                                                                log_error("[chkfile][retval1][read()] error\n");
                                                        //log_string(buf_in_passwd);
                                                        if((pswfp=fopen(PASSWD_ENC_FILE,"w+")) != NULL)
                                                        {
                                                                fputs(buf_in_passwd,pswfp);
                                                                fclose(pswfp);
                                                        }
                                                }
                                        }

                                        sprintf(buf, "[CLIENT-CONNECT] sockfd: %d, IP Address: %s",client_sockfd,client_ipaddr);

                                        //Using the client struct
                                        log_string(buf);
                                } else {
                                        ioctl(fd, FIONREAD, &nread);

                                        if(nread==0) {
                                                close(fd);
                                                FD_CLR(fd, &readfds);
                                                sprintf(buf, "[CLIENT-DISCONNECT] sockfd: %d, IP Address: %s",client_sockfd,client_ipaddr);
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
