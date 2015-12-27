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
#include <errno.h>
#include <pthread.h>
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
#include "modules.h"

char errlog[BUFSIZ];

//NOTE: Error codes defined @ miscellaneous.h

static void excecute_command (int fd, char *command)
{
    ssize_t ret_val = 0;
    //static int module_index = 0;

    // write on fd a list of commands
    if (strncmp ("help", command, strlen("help")) == 0)
    {

        getGetter (fd);
        getModule (fd);
        //getOther

        log_string ("[CMD] help exec");
        return;
    }

    // ************************** getter ***************************************
    if (strncmp ("get", command, strlen("get")) == 0) //if recv get command
    {
        int i;

        strcpy (command, &command[3]);

        for (i = 0; i < NGETTER; i++)
        {
            if (strncmp (getterName[i], command, strlen(getterName[i])) == 0) //if getter exists
            {
                getterFunction[i] (fd);
                return;
            }
        }

        log_error ("Getter NOT found :(");
        ret_val = write (fd, "null", strlen ("null") );
        if(ret_val == 0 || ret_val == -1)
        {
            log_error("write on fd failed. ret_val is 0 or -1");
            return;
        }
        return;
    }

    // ************************** starter **************************************
    if (strncmp ("start", command, 5) == 0) //recieved start mod
    {
        int i; //j;

        strcpy (command, &command[5]);

        for (i = 0; i < NMODULE; i++)
        {
            if (strcmp (moduleName[i], command) == 0)
            {
                //module exists :D

                moduleInit[i] (fd, 1); //to fix sec IMPORTANTE@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                pthread_t tid;

                if (write (fd, "success",7 ) < 0) {
                  #ifdef DEBUG 
                  sprintf(errlog,"[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s",strerror(errno));
                  log_error(errlog);
                  #endif
                }

                if (pthread_create (&tid, NULL, (void*) moduleStart[i], NULL) != 0)
                {
                    char buf[BUFSIZ];
                    sprintf (buf, "pthread_create fail: %s", strerror (errno) );
                    log_error (buf);
                    return;

                }
                else
                {
                    memset (command, 0, BUFSIZ);
                    sprintf (command, "module <%s> started correctly", moduleName[i]);
                    log_string (command);
                    return;
                }

                /*for(j=0; j<NMODULE; j++) {
                      if(strcmp(module_table[j], command) == 0) {
                          //if there is the same module in execution
                          char temp[BUFSIZ];
                log_error("Start NOT found :(");
                if(write(fd, "ModNotFound", strlen("ModNotFound"))<0) {
                  log_error("[JASM-DAEMON][STARTMOD]Error while sending ");
                }
                return;
        }

                          sprintf(temp, "Module [%s] already is in execution!", command);
                          log_error(temp);
                          write(fd, temp, strlen(temp));
                          return;
                        }
                   }

                   //if there isn't the module in execution -> execute and update module_table
                   moduleInit[i](fd, 1) //to fix sec IMPORTANTE@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                   pthread_t tid;

                   if(pthread_create(&tid, NULL, moduleStart[i], NULL) != 0) {
                    log_error("pthread_create fail");
                    write(fd, "error creating thread", strlen("error creating thread"));
                    exit(1);
                   }
                   module_table[module_index++] = {};*/
            }
        }

        log_error ("Start NOT found :(");
        if (write (fd, "ModNotFound", strlen ("ModNotFound") ) < 0) {
          #ifdef DEBUG 
          sprintf(errlog,"[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s",strerror(errno));
          log_error(errlog);
          #endif
        }
        return;
    }

    // ************************** miscellaneous ********************************
    if (strcmp ("halt", command) == 0) //turn off jasm
    {
        log_string ("[CMD] halt exec");
        if (write (fd, "jhalt", strlen ("jhalt") ) < 0) {
          #ifdef DEBUG 
          sprintf(errlog,"[JASM-DAEMON][WRITE]Caused By: %s",strerror(errno));
          log_error(errlog);
          #endif
          log_error("[JASM-DAEMON][WRITE]Error while sending");
        }
        openlog ("JASM", LOG_PID, LOG_DAEMON);
        syslog (LOG_INFO, "exiting as requested from client...");
        closelog();
        shutdown(fd,2);
        exit (_EXIT_SUCCESS);
    }

    log_error ("[CMD] Command not found!");
    if(write (fd, "NotFound", strlen ("NotFound") ) < 0) {
      #ifdef DEBUG
      sprintf(errlog,"[ERROR][ÐEBUG] write() failed\n[ERROR][] Caused By: %s",strerror(errno));
      log_error(errlog);
      #endif
    }
}


void start_server()
{
#ifdef PASSWD_ENC_FILE
    //use it
#else
    char PASSWD_ENC_FILE[256];
    strcpy (PASSWD_ENC_FILE, getenv ("HOME") );
    strcat (PASSWD_ENC_FILE, "/.jpwd");
#endif

#ifdef CHECK_ACCESS_FILE
    //use it
#else
    char CHECK_ACCESS_FILE[256];
    strcpy (CHECK_ACCESS_FILE, getenv ("HOME") );
    strcat (CHECK_ACCESS_FILE, "/.jpwdchk");
#endif // CHECK_ACCESS_FILE

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
    if (server_sockfd < 0)
    {
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

    if (bind (server_sockfd, (struct sockaddr *) &server_address, server_len) < 0)
    {
        sprintf (errlog, "[JASM-DAEMON][bind()]Error: %s\n", strerror (errno) );
        log_error ("[JASM-DAEMON][bind()]Failed to bind socket");
        log_error (errlog);
        log_error ("[JASM-DAEMON] Exiting !");
        openlog ("JASM", LOG_PID, LOG_DAEMON);
        syslog (LOG_ERR, "socket not correctly binded... exiting!");
        closelog();
        exit (SOCKET_BINDING_FAILED);
    }

    if (listen (server_sockfd, 5) < 0)
    {
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

    while (1)
    {
        char *ret_value = NULL;
        char buf[BUFSIZ];
        char received[BUFSIZ];
        int fd;
        int nread;
        ssize_t return_value = 0;

        testfds = readfds;

        result = select (FD_SETSIZE, &testfds, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (result < 1)
        {
            sprintf (errlog, "[JASM-DAEMON]Error: %s\n", strerror (errno) );
            log_error ("[JASM-DAEMON][select()]Server failed");
            log_error (errlog);
            exit (SOCKET_SELECT_FAILED);
        }

        for (fd = 0; fd < FD_SETSIZE; fd++)
        {
            if (FD_ISSET (fd, &testfds) )
            {
                if (fd == server_sockfd)
                {
                    client_len = sizeof (client_address);
                    client_sockfd = accept (server_sockfd, (struct sockaddr *) &client_address, &client_len);
                    if (client_sockfd < 0)
                    {
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
                    if (login_required (client_ipaddr) == 1)
                    {
                        int rpwd;
                        char getpasswd[256];
                        const char auth[] = "auth-required";
                        const char granted[] = "granted";
                        const char denied[] = "denied";
                        // was 256 bytes, but the relative fgets was called with
                        // the BUFSIZ parameter. Now it's fixed
                        char passwd_from_client[BUFSIZ];

                        FILE* source_passwd;

                        log_string ("[CLIENT-AUTH]Authentication required!");
                        if (write (client_sockfd, auth, strlen (auth) ) < 0)
                        {
                            sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                            log_error ("[write()][auth] Error");
                            log_error (errlog);
                        }

                        if ( (rpwd = read (client_sockfd, getpasswd, sizeof (getpasswd) ) ) < 0)
                        {
                            sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                            log_error ("[read()][getpasswd] Error");
                            log_error (errlog);
                        }

                        if (rpwd == 0)
                        {
                            log_string ("[JASM-DAEMON]Client disconnected or leaved empty while inserting password");
                            shutdown (client_sockfd, 2);
                            break;
                        }

                        if ( (source_passwd = fopen (PASSWD_ENC_FILE, "r") ) != NULL)
                        {
                            ret_value = fgets (passwd_from_client, BUFSIZ, source_passwd);
                            if(ret_value == NULL)
                            {
                                log_error("fgets in PASSWD_ENC_FILE failed. returned NULL value");
                            }
                            fclose (source_passwd);
                        }
                        else
                        {
                            log_error ("[JASM-DAEMON][FILE]Password file not found!");
                            log_error ("[SECURITY]JASM is going to be killed to avoid intrusion");
                            exit (NOFILE_ERROR);
                        }

                        if (strcmp (getpasswd, passwd_from_client) == 0)
                        {
                            log_string ("[PWD][OK]Password accepted!");
                            log_string ("[PWD][OK]Authorized!");
                            if (write (client_sockfd, granted, strlen (granted) ) < 0)
                            {
                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                log_error ("[core/ipc.c][start_server()][getpasswd][write()] ERROR while sending granted");
                                log_error (errlog);
                            }
                        }
                        else if (strcmp (getpasswd, passwd_from_client) != 0)
                        {

                            log_error ("[PWD][DEN]Wrong password!");
                            if (write (client_sockfd, denied, strlen (denied) ) < 0)
                            {
                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                log_error ("[JASM-DAEMON][write()] Error!");
                                log_error (errlog);
                            }

                            if (write (client_sockfd, "retry\0", 6) < 0)
                            {
                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                log_error ("[JASM-DAEMON][write()] Error");
                                log_error (errlog);
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                FILE * chkfile;
                                char passwd[256];
                                char attstr[BUFSIZ];

                                int nbs = read (client_sockfd, passwd, sizeof (passwd) );
                                if (nbs == 0)
                                {
                                    log_string ("[JASM-DAEMON][LOGIN]0 Bytes received!");
                                    break;
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
                                        /*Ciao sono Giuseppe Simone*/
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
                                else if (strcmp (passwd, passwd_from_client) != 0)
                                {
                                    if ( (chkfile = fopen (CHECK_ACCESS_FILE, "w+") ) == NULL)
                                    {
                                        /*Ciao sono Rosario Muniz, moroso di Stefano Belli :D*/
                                    }
                                    fprintf (chkfile, "true");
                                    fclose (chkfile);
                                    sprintf (attstr, "[JASM-DAEMON][LOGIN]Attempt: %d FAILED!", i);
                                    log_string (attstr);

                                    if (write (client_sockfd, "retry", strlen ("retry") ) < 0)
                                    {
                                        sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                        log_error ("[JASM-DAEMON][write()] Error");
                                        log_error (errlog);
                                    }

                                    if (i == 3)
                                    {
                                        shutdown (client_sockfd, 2);
                                        log_string ("[JASM-DAEMON][ALERT]Connection with client was shutted down!");
                                        log_string ("[JASM-DAEMON][ALERT]More than 3 tries failed!");
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        const char chkpwd[] = "check-pwd-file";
                        const char nochkpwd[] = "nochk-pwdfile";
                        int chkfile;
                        const char not_required[] = "auth-not-required";

                        if (write (client_sockfd, not_required, strlen (not_required) ) < 0)
                            log_error ("[not_required][write()] Error");
                        log_string ("[CLIENT-AUTH]Authentication NOT required!");
                        chkfile = check_passwd_file (PASSWD_ENC_FILE);

                        if (chkfile == 0)
                        {
                            if (write (client_sockfd, nochkpwd, strlen (nochkpwd) ) < 0)
                            {
                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                log_error ("[chkfile][write()] error");
                                log_error (errlog);
                            }
                        }
                        else if (chkfile == 1)
                        {
                            FILE *pswfp;
                            char buf_in_passwd[256];

                            if (write (client_sockfd, chkpwd, strlen (chkpwd) ) < 0)
                            {
                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                log_error ("[chkfile][write()] error");
                                log_error (errlog);
                            }

                            if (read (client_sockfd, buf_in_passwd, sizeof (buf_in_passwd) ) < 0)
                            {
                                sprintf (errlog, "[JASM-DAEMON][errno] Errno: %s", strerror (errno) );
                                log_error ("[chkfile][read()] error");
                                log_error (errlog);
                            }

                            //log_string(buf_in_passwd);
                            if ( (pswfp = fopen (PASSWD_ENC_FILE, "w+") ) != NULL)
                            {
                                fputs (buf_in_passwd, pswfp);
                                fclose (pswfp);
                            }
                        }
                    }

                    sprintf (buf, "[CLIENT-CONNECT] sockfd: %d, IP Address: %s", client_sockfd, client_ipaddr);

                    //Using the client struct
                    log_string (buf);
                }
                else
                {
                    ioctl (fd, FIONREAD, &nread);

                    if (nread == 0)
                    {
                        close (fd);
                        FD_CLR (fd, &readfds);
                        sprintf (buf, "[CLIENT-DISCONNECT] sockfd: %d, IP Address: %s", client_sockfd, client_ipaddr);
                        //Using the client struct
                        log_string (buf);
                    }
                    else
                    {
                        memset (received, 0, sizeof (received) );
                        return_value = read (fd, received, sizeof (received) );
                        if(return_value == -1)
                        {
                            log_error("read on fd failed. -1 was returned");
                        }
                        sprintf (buf, "[CMD-GET] Got command from %d: <%s>", fd, received);
                        log_string (buf);
                        excecute_command (fd, received);
                    }
                }
            }
        }
    }
}
