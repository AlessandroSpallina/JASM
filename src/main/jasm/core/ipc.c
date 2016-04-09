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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __unix__
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <syslog.h>
#endif

#include "jasmbuild_info.h"
#include "configfile.h"
#include "ipc.h"
#include "miscellaneous.h"
#include "getter.h"
#include "modules.h"
#include "modules_list.h"
#include "logger.h"

struct ip_node *client_list = NULL;

unsigned int connection_counter = 0;
char errlog[BUFSIZ];

//NOTE: Error codes defined @ miscellaneous.h

static void excecute_command (int fd, char *ip, char *command)
{
        ssize_t ret_val = 0;

        // write on fd a list of commands
        if (strncmp ("help", command, strlen ("help") ) == 0) {

                getGetter (fd);
                getModule (fd);
                //getOther

                log_string ("[CMD] help exec");
                return;
        }

        // ************************** getter ***************************************
        if (strncmp ("get", command, strlen ("get") ) == 0) { //if recv get command
                int i;

                strcpy (command, &command[3]);

                for (i = 0; i < NGETTER; i++)
                {
                        if (strncmp (getterName[i], command, strlen (getterName[i]) ) == 0) //if getter exists
                        {
                                getterFunction[i] (fd);
                                return;
                        }
                }

                log_error ("Getter NOT found :(");
                ret_val = write (fd, "null", strlen ("null") );
                if (ret_val == 0 || ret_val == -1)
                {
                        sprintf(errlog,"[JASM-DAEMON][ERROR][write()] Error for write : %s",strerror(errno));
                        log_error(errlog);
                }
                return;
        }

        // ************************** starter **************************************
        if (strncmp ("start", command, strlen ("start") ) == 0) { //recieved start mod
                int i;

                strcpy (command, &command[5]);

                for (i = 0; i < NMODULE; i++) {
                        if (strcmp (moduleName[i], command) == 0) {
                                //module exists :D

                                struct ip_node *client = find_clientIp(client_list, ip);
                                if(client != NULL) {
                                        struct module_running *module = find_module_running(client->modules_list, command);
                                        if(module == NULL) {

                                                moduleInit[i] (fd, 1); //to fix sec IMPORTANTE@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                                                pthread_t tid;

                                                if (write (fd, "success", 7 ) < 0) {
                                                        sprintf (errlog, "[ERROR][DEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno));
                                                        log_error (errlog);
                                                }

                                                if (pthread_create (&tid, NULL, (void *) moduleStart[i], NULL) != 0) {
                                                        char buf[BUFSIZ];
                                                        sprintf (buf, "pthread_create fail: %s", strerror (errno) );
                                                        log_error (buf);
                                                        return;

                                                } else {
                                                        memset (command, 0, BUFSIZ);
                                                        sprintf (command, "module <%s> started correctly", moduleName[i]);
                                                        log_string (command);
                                                        add_module_running(&client->modules_list, command, tid);
                                                        return;
                                                }

                                        } else {
                                                sprintf(errlog, "[ERROR] unable to load <%s> module, no double modules are allow", command);
                                                log_string(errlog);
                                                if (write (fd, "fail", 4) < 0) {
                                                        sprintf (errlog, "[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno) );
                                                        log_error (errlog);
                                                }
                                                return;
                                        }
                                } else {
                                        sprintf(errlog, "[ERROR] client ip not found in client_list");
                                        log_error(errlog);
                                        return;
                                }
                        }
                }

                log_error ("Start NOT found :(");
                if (write (fd, "ModNotFound", strlen ("ModNotFound") ) < 0) {
                        sprintf (errlog, "[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno) );
                        log_error (errlog);
                }
                return;
        }

        // ************************** stopper **************************************
        if (strncmp ("stop", command, strlen ("stop") ) == 0) {
                strcpy (command, &command[4]);
                struct ip_node *client = find_clientIp(client_list, ip);
                if(client != NULL) {
                        if (client->modules_list != NULL) {
                                struct module_running *aus = find_module_running(client->modules_list, command);
                                if (aus != NULL) {

                                        if (pthread_key_delete(aus->tid) == 0) {
                                                log_string("[STOPPER] JASM stopped the module successfully");
                                                rem_module_running(&(client->modules_list), command);
                                                if (write(fd, "success", 7) < 0) {
                                                        sprintf (errlog, "[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno) );
                                                        log_error (errlog);
                                                }
                                                return;

                                        } else {
                                                log_error("[STOPPER] Unable to delete module's thread, pthread_key_delete fail");
                                                if (write(fd, "fail", 4) < 0) {
                                                        sprintf (errlog, "[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno) );
                                                        log_error (errlog);
                                                }
                                                return;
                                        }
                                } else {
                                        log_error("[STOPPER] Unable to find module to stop!");
                                        if (write(fd, "fail", 4) < 0) {
                                                sprintf (errlog, "[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno) );
                                                log_error (errlog);
                                        }
                                        return;
                                }

                        } else {
                                log_error("[STOPPER] Client haven't any module running: unable to stop");
                                if (write(fd, "fail", 4) < 0) {
                                        sprintf (errlog, "[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno) );
                                        log_error (errlog);
                                }
                                return;
                        }

                } else {
                        log_error("[STOPPER] Unable to find client - in heap - node");
                        if (write(fd, "fail", 4) < 0) {
                                sprintf (errlog, "[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno) );
                                log_error (errlog);
                        }
                        return;
                }

        }

        // ************************** miscellaneous ********************************
        if (strcmp ("halt", command) == 0) { //turn off jasm
                log_string ("[CMD] halt exec");
                if (write (fd, "jhalt", strlen ("jhalt") ) < 0) {
                        sprintf (errlog, "[JASM-DAEMON][WRITE]Caused By: %s", strerror (errno) );
                        log_error (errlog);
                }
                openlog ("JASM", LOG_PID, LOG_DAEMON);
                syslog (LOG_INFO, "exiting as requested from client...");
                closelog();
                shutdown (fd, 2);
                exit (_EXIT_SUCCESS);
        }

        log_error ("[CMD] Command not found!");
        if (write (fd, "NotFound", strlen ("NotFound") ) < 0) {
                sprintf (errlog, "[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s", strerror (errno) );
                log_error (errlog);
        }
}

static _Bool goodLoginRoutine(char *client_ipaddr, int client_sockfd)
{
        char buf[BUFSIZ];

        sprintf (buf, "[CLIENT-CONNECT] sockfd: %d, IP Address: %s", client_sockfd, client_ipaddr);
        log_string (buf);
        add_clientIp(&client_list, client_ipaddr);
#ifdef DEBUG
        log_client(client_list);
#endif
        if (connection_counter <= *(int*)_config[CONFIG_MAX_CONNECTIONS].config_values) {
                connection_counter++;
#ifdef DEBUG
                log_string("client connection is ok");
                sprintf(errlog,"Client: %d",connection_counter);
                log_string(errlog);
#endif

                return true;

        } else {
                //if (connection_counter > *(int*)_config[CONFIG_MAX_CONNECTIONS].config_values)
                shutdown(client_sockfd,2);
#ifdef DEBUG
                log_string("client connection refused: max connection limit hit!");
                sprintf(errlog,"Client: %d",connection_counter);
                log_string(errlog);
#endif
                return false;
        }

}


static _Bool isCorrectPassword(int client_sockfd)
{
#ifndef PASSWD_ENC_FILE
        char PASSWD_ENC_FILE[256];
        strcpy (PASSWD_ENC_FILE, getenv ("HOME") );
        strcat (PASSWD_ENC_FILE, "/.jpwd");
#endif

#ifndef CHECK_ACCESS_FILE
        char CHECK_ACCESS_FILE[256];
        strcpy (CHECK_ACCESS_FILE, getenv ("HOME") );
        strcat (CHECK_ACCESS_FILE, "/.jpwdchk");
#endif // CHECK_ACCESS_FILE
        int rpwd;
        char auth[BUFSIZ], granted[BUFSIZ], denied[BUFSIZ];
        char passwd_from_client[BUFSIZ], getit[BUFSIZ], getpasswd[256];

        strncpy(auth,"auth-required",13);
        strncpy(granted,"granted",7);
        strncpy(denied,"denied",6);

        FILE* source_passwd;

        log_string ("[CLIENT-AUTH]Authentication required!");
        if (write (client_sockfd, auth, strlen (auth) ) < 0) {
                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                log_error ("[write()][auth] Error");
                log_error (errlog);
        }
        
        if ( (rpwd = read (client_sockfd, getpasswd, sizeof (getpasswd) ) ) < 0) {
                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                log_error ("[read()][getpasswd] Error");
                log_error (errlog);
        } else {
                if (rpwd == 0) {
                        log_string ("[JASM-DAEMON]Client disconnected or leaved empty while inserting password");
                        shutdown (client_sockfd, 2);
                }
        }

        if ( (source_passwd = fopen (PASSWD_ENC_FILE, "r") ) != NULL) {
                char *ret_value = fgets (passwd_from_client, BUFSIZ, source_passwd);
                if (ret_value == NULL)
                {
                        log_error ("fgets in PASSWD_ENC_FILE failed. returned NULL value: exiting");
                        fclose (source_passwd);
                        exit (NOFILE_ERROR);
                }
                fclose (source_passwd);
        } else {
                log_error ("[JASM-DAEMON][FILE]Password file not found!");
                log_error ("[SECURITY]JASM is going to be killed to avoid intrusion");
                exit (NOFILE_ERROR);
        }
        if (strcmp (getpasswd, passwd_from_client) == 0) {
                log_string ("[PWD][OK]Password accepted!");
                log_string ("[PWD][OK]Authorized!");
                if (write (client_sockfd, granted, strlen (granted) ) < 0) {
                        sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                        log_error ("[core/ipc.c][start_server()][getpasswd][write()] ERROR while sending granted");
                        log_error (errlog);
                }
                return true;

        } else  {

                log_error ("[PWD][DEN]Wrong password!");
                if (write (client_sockfd, denied, strlen (denied) ) < 0) {
                        sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                        log_error ("[JASM-DAEMON][write()] Error!");
                        log_error (errlog);
                }

                if(read (client_sockfd,getit,strlen(getit)) < 0) {
                        sprintf(errlog,"[JASM-DAEMON][errno] Errno: %s",strerror(errno));
                        log_error("[JASM-DAEMON][read()] Error!");
                        log_error(errlog);
                }

                if (write (client_sockfd, "retry", 5) < 0) {
                        sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                        log_error ("[JASM-DAEMON][write()] Error");
                        log_error (errlog);
                }

                for (int i = 1; i <= *(int*)_config[CONFIG_MAX_AUTHENTICATION_TRIES].config_values + 1; i++) {
                        FILE * chkfile;
                        char passwd[256];
                        char attstr[BUFSIZ];

                        int nbs = read (client_sockfd, passwd, sizeof (passwd) );
                        if (nbs == 0) {
                                log_string ("[JASM-DAEMON][LOGIN]0 Bytes received!");
                                shutdown (client_sockfd, 2);

                        }
                        else if (nbs < 0)
                        {
                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                log_error ("[JASM-DAEMON][read()] Error");
                                log_error (errlog);
                        }

                        if (strcmp (passwd, passwd_from_client) == 0)
                        {
                                if ( (chkfile = fopen (CHECK_ACCESS_FILE, "w+") ) == NULL)
                                {
                                        log_error ("fgets in PASSWD_ENC_FILE failed. returned NULL value: exiting");
                                        fclose (source_passwd);
                                        exit (NOFILE_ERROR);
                                }
                                fprintf (chkfile, "falset");
                                fclose (chkfile);
                                sprintf (attstr, "[JASM-DAEMON][LOGIN]Attempt: %d SUCCESS!", i);
                                log_string (attstr);
                                if (write (client_sockfd, "authorized", strlen ("authorized") ) < 0)
                                {
                                        sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                        log_error ("[JASM-DAEMON][write()] Error");
                                        log_error (errlog);
                                }
                                break;
                        }
                        else if (strcmp (passwd, passwd_from_client) != 0) {
                                if ( (chkfile = fopen (CHECK_ACCESS_FILE, "w+") ) == NULL) {
                                        log_error ("fgets in PASSWD_ENC_FILE failed. returned NULL value: exiting");
                                        fclose (source_passwd);
                                        exit (NOFILE_ERROR);
                                }
                                fprintf (chkfile, "true");
                                fclose (chkfile);
                                sprintf (attstr, "[JASM-DAEMON][LOGIN]Attempt: %d FAILED!", i);
                                log_string (attstr);

                                if (write (client_sockfd, "retry", strlen ("retry") ) < 0) {
                                        sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                        log_error ("[JASM-DAEMON][write()] Error");
                                        log_error (errlog);
                                }

                                if (i == *(int*)_config[CONFIG_MAX_AUTHENTICATION_TRIES].config_values) {
                                        char intalert[BUFSIZ];
                                        shutdown (client_sockfd, 2);
                                        log_string ("[JASM-DAEMON][ALERT]Connection with client was shutted down!");
                                        sprintf(intalert,"[JASM-DAEMON][ALERT]More than %d tries failed",*(int*)_config[CONFIG_MAX_AUTHENTICATION_TRIES].config_values);

                                }
                        }
                }
        }
        return false;
}

void start_server()
{
#ifndef PASSWD_ENC_FILE
        char PASSWD_ENC_FILE[256];
        strcpy (PASSWD_ENC_FILE, getenv ("HOME") );
        strcat (PASSWD_ENC_FILE, "/.jpwd");
#endif
        int server_sockfd, client_sockfd;
        int server_len;
        socklen_t client_len;
        struct sockaddr_in server_address;
        struct sockaddr_in client_address;
        int result;
        char client_ipaddr[30];

        fd_set readfds, testfds;

        client_sockfd = 0;

        server_sockfd = socket (AF_INET, SOCK_STREAM, 0);
        if (server_sockfd < 0) {
                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                log_error ("[JASM-DAEMON][socket()]Failed to create new socket! Exiting...");
                log_error (errlog);
                openlog ("JASM", LOG_PID, LOG_DAEMON);
                syslog (LOG_ERR, "socket creation failed!! Exiting!");
                closelog();
                exit (SOCKET_CREATION_FAILED);
        }
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = htonl (INADDR_ANY);
        server_address.sin_port = htons (SERVER_PORT);
        server_len = sizeof (server_address);

        if (bind (server_sockfd, (struct sockaddr *) &server_address, server_len) < 0) {
                sprintf (errlog, "[JASM-DAEMON][bind()]Error: %s\n", strerror (errno) );
                log_error ("[JASM-DAEMON][bind()]Failed to bind socket");
                log_error (errlog);
                log_error ("[JASM-DAEMON] Exiting !");
                openlog ("JASM", LOG_PID, LOG_DAEMON);
                syslog (LOG_ERR, "socket not correctly binded... exiting!");
                closelog();
                exit (SOCKET_BINDING_FAILED);
        }

        if (listen (server_sockfd, 5) < 0) {
                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                log_error ("[JASM-DAEMON][listen()]Failed to put socket in listening mode!");
                log_error (errlog);
                openlog ("JASM", LOG_PID, LOG_DAEMON);
                syslog (LOG_ERR, "FATAL! socket was not put to listening mode! Exiting...");
                closelog();
                exit (SOCKET_LISTENING_CONNECTION_FAILED);
        }

        FD_ZERO (&readfds);
        FD_SET (server_sockfd, &readfds);

        log_string ("[JASM-DAEMON]Server started");

        while (1) {
                //char *ret_value = NULL;
                char buf[BUFSIZ];
                char received[BUFSIZ];
                int fd;
                int nread;
                ssize_t return_value = 0;

                testfds = readfds;

                result = select (FD_SETSIZE, &testfds, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

                if (result < 1) {
                        sprintf (errlog, "[JASM-DAEMON]Error: %s\n", strerror (errno) );
                        log_error ("[JASM-DAEMON][select()]Server failed");
                        log_error (errlog);
                        exit (SOCKET_SELECT_FAILED);
                }

                for (fd = 0; fd < FD_SETSIZE; fd++) {
                        if (FD_ISSET (fd, &testfds) ) {
                                if (fd == server_sockfd) {
                                        client_len = sizeof (client_address);
                                        client_sockfd = accept (server_sockfd, (struct sockaddr *) &client_address, &client_len);
                                        if (client_sockfd < 0) {
                                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                                log_error ("[JASM-DAEMON][accept()]Failed to accept socket connection!");
                                                log_error (errlog);
                                                openlog ("JASM", LOG_PID, LOG_DAEMON);
                                                syslog (LOG_ERR, "FATAL! Failed to accept client incoming connection! Exiting...");
                                                closelog();
                                                exit (SOCKET_CLIENT_CONNECTION_FAILED);
                                        }
                                        FD_SET (client_sockfd, &readfds);
                                        sprintf (client_ipaddr, "%d.%d.%d.%d", client_address.sin_addr.s_addr & 0xFF, (client_address.sin_addr.s_addr & 0xFF00) >> 8, (client_address.sin_addr.s_addr & 0xFF0000) >> 16, (client_address.sin_addr.s_addr & 0xFF000000) >> 24);
                                        //************************************************
                                        if(login_required(client_ipaddr)) {
                                               
                                        	if(isCorrectPassword(client_sockfd)) {
                                                	goodLoginRoutine(client_ipaddr, client_sockfd);
                                                } else {
                                                	//if here -> too many wrongs
                                                	shutdown (client_sockfd, 2);
                                                	sprintf(errlog, "[WARNING] Client %s insert too many wrong passwords. Closing sockets, be careful man!", client_ipaddr);
                                                	log_string(errlog);
                                                }
                                        } else {
                                                // change assignment method ( using strncpy() )
                                                const char chkpwd[] = "check-pwd-file";
                                                const char nochkpwd[] = "nochk-pwdfile";
                                                int chkfile;
                                                const char not_required[] = "auth-not-required";

                                                if (write (client_sockfd, not_required, strlen (not_required) ) < 0)
                                                        log_error ("[not_required][write()] Error");

                                                log_string ("[CLIENT-AUTH]Authentication NOT required!");
                                                log_string("trymeee");
                                                chkfile = check_passwd_file (PASSWD_ENC_FILE);

                                                if (chkfile == 0) {
                                                        if (write (client_sockfd, nochkpwd, strlen (nochkpwd) ) < 0) {
                                                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                                                log_error ("[chkfile][write()] error");
                                                                log_error (errlog);
                                                        }
                                                }
                                                else if (chkfile == 1) {
                                                        FILE *pswfp;
                                                        char buf_in_passwd[256];
                                                        int bytes;

                                                        if (write (client_sockfd, chkpwd, strlen (chkpwd) ) < 0) {
                                                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                                                log_error ("[chkfile][write()] error");
                                                                log_error (errlog);
                                                        }

                                                        bytes = read (client_sockfd, buf_in_passwd, sizeof (buf_in_passwd));
                                                        if(bytes < 0) {
                                                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                                                log_error ("[chkfile][read()] error");
                                                                log_error (errlog);
                                                        //}else if (bytes == 0) {
                                                                // Do nothing
                                                        } else if(bytes > 0) {
                                                                if ( (pswfp = fopen (PASSWD_ENC_FILE, "w+") ) != NULL) {
                                                                        fputs (buf_in_passwd, pswfp);
                                                                        fclose (pswfp);
                                                                }
                                                        }
                                                }
                                                goodLoginRoutine(client_ipaddr, client_sockfd);
                                        }

                                } else {
                                        ioctl (fd, FIONREAD, &nread);

                                        if (nread == 0) {
                                                close (fd);
                                                FD_CLR (fd, &readfds);
                                                sprintf (buf, "[CLIENT-DISCONNECT] sockfd: %d, IP Address: %s", client_sockfd, client_ipaddr);
                                                log_string (buf);
                                                rem_clientIp(&client_list, client_ipaddr);
                                                connection_counter--;

#ifdef DEBUG
                                                log_client(client_list);
#endif
                                        } else {
                                                memset (received, 0, sizeof (received) );
                                                return_value = read (fd, received, sizeof (received) );
                                                if (return_value == -1) {
                                                        log_error ("read on fd failed. -1 was returned");
                                                }
                                                sprintf (buf, "[CMD-GET] Got command from %d: <%s>", fd, received);
                                                log_string (buf);
                                                excecute_command (fd, client_ipaddr, received);
                                        }
                                }
                        }
                }
        }

}
