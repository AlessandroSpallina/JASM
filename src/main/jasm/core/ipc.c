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
#include <ctype.h>
#include <errno.h>

#ifdef __unix__
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <syslog.h>
#endif

#include "ipc.h"
#include "miscellaneous.h"
#include "logger.h"
#include "macros.h"

static inline ssize_t jasm_write(int sockfd, const char* __src, const char* data_type);
static inline ssize_t jasm_read(int sockfd, char* body);
static inline ssize_t jasm_read_with_header(int sockfd, char* body, char* header);
static inline ssize_t jasm_write_with_header(int sockfd, const char* __src, const char* data_type, const char* header);

//static struct ip_node *client_list = NULL;

static void execute_command (int fd, char *command)
{
	// write on fd a list of commands
	if (!strncmp (command,"help",4)) {
		jasm_write(fd,"HelpRequested",DATA_TEXT);
		return;
	}
	else if (!strncmp(command,"halt",4)) { //turn off jasm
        jasm_write(fd,"HaltingOperationRunning",DATA_TEXT);
		wlogev(EV_INFO, "Halting as requested... Bye bye");
		openlog ("JASM", LOG_PID, LOG_DAEMON);
		syslog (LOG_INFO, "exiting as requested from client...");
		closelog();
		shutdown(fd,2);
		exit (_EXIT_SUCCESS);
	}
	jasm_write(fd,"NotFound",DATA_TEXT);
    
	/*
	   // ************************** getter ***************************************
	   if (!strncmp ("get", command, 3)) { //if recv get command
	        int i;

	        strncpy (command, &command[3]);

	        for (i = 0; i < NGETTER; i++){
	                if (strncmp (getterName[i], command, strlen (getterName[i]) ) == 0) {
	                        getterFunction[i] (fd);
	                        return;
	                }
	        }

	        sendMsg(fd,"null");
	        return;
	   }

	   // ************************** starter **************************************
	   if (strncmp ("start", command, strlen ("start") ) == 0) { //recieved start mod
	        int i;

	        strncpy (command, &command[5]);

	        for (i = 0; i < NMODULE; i++) {
	                if (strncmp (moduleName[i], command) == 0) {
	                        //module exists :D

	                        struct ip_node *client = find_clientIp(client_list, ip);
	                        if(client != NULL) {
	                                struct module_running *module = find_module_running(client->modules_list, command);
	                                if(module == NULL) {

	                                        moduleInit[i] (fd, 1); //to fix sec IMPORTANTE@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	                                        pthread_t tid;

	                                        sendMsg(fd,"success");

	                                        if (pthread_create (&tid, NULL, (void*)moduleStart[i], NULL) != 0) {
	                                                char buf[BUFSIZ];
	                                                snprintf (buf, "pthread_create fail: %s", strerror (errno) );
	                                                wlogev (EV_ERROR, buf);
	                                                return;

	                                        } else {
	                                                memset (command, 0, BUFSIZ);
	                                                snprintf (command, "module <%s> started correctly", moduleName[i]);
	                                                wlogev (EV_INFO, command);
	                                                add_module_running(&client->modules_list, command, tid);
	                                                return;
	                                        }

	                                } else {
	                                        snprintf(errlog, "[ERROR] unable to load <%s> module, no double modules are allow", command);
	                                        wlogev (EV_ERROR, errlog);
	                                        sendMsg(fd,"fail");
	                                        return;
	                                }
	                        } else {
	                                snprintf(errlog, "[ERROR] client ip not found in client_list");
	                                wlogev (EV_ERROR, errlog);
	                                return;
	                        }
	                }
	        }

	        sendMsg(fd,"CannotFindModule");
	        return;
	   }

	   // ************************** stopper **************************************
	   if (strncmp ("stop", command, strlen ("stop") ) == 0) {
	        strncpy (command, &command[4]);
	        struct ip_node *client = find_clientIp(client_list, ip);
	        if(client != NULL) {
	                if (client->modules_list != NULL) {
	                        struct module_running *aus = find_module_running(client->modules_list, command);
	                        if (aus != NULL) {

	                                if (pthread_key_delete((pthread_key_t)aus->tid) == 0) {
	                                        wlogev(EV_INFO, "[STOPPER] JASM stopped the module successfully");
	                                        rem_module_running(&(client->modules_list), command);
	                                        sendMsg(fd,"success");
	                                        return;

	                                } else {
	                                        wlogev(EV_ERROR, "[STOPPER] Unable to delete module's thread, pthread_key_delete fail");

	                                        sendMsg(fd,"CannotStopModule_ThreadDeleteFailuire");
	                                        return;
	                                }
	                        } else {
	                                wlogev(EV_ERROR, "[STOPPER] Unable to find module to stop!");
	                                sendMsg(fd,"CannotStopModule_NotFound");
	                                return;
	                        }

	                } else {
	                        wlogev(EV_ERROR, "[STOPPER] Client haven't any module running: unable to stop");
	                        sendMsg(fd,"CannotStopModule_NoModuleRunning");
	                        return;
	                }

	        } else {
	                wlogev(EV_ERROR, "[STOPPER] Unable to find client - in heap - node");
	                sendMsg(fd,"CannotStopModule_ClientNotFound");
	                return;
	        }

	   }
	 */
}

void start_server()
{
	int server_sockfd=-1, client_sockfd=-1, server_len, result=-1;
	socklen_t client_len;
	struct sockaddr_in server_address, client_address;
	char client_ipaddr[30];

	fd_set readfds, testfds;
	server_sockfd = socket (AF_INET, SOCK_STREAM, 0);
	if (server_sockfd < 0) {
		wlogev(EV_ERROR, "Socket creation failure: %s",strerror(errno));
		openlog ("JASM", LOG_PID, LOG_DAEMON);
		syslog (LOG_ERR, "socket creation failed!! Exiting!");
		closelog();
		exit (SOCKET_CREATION_FAILED);
	}

	int y=1;
	if(setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,(char*)&y,sizeof(y)) < 0) {
		wlogev(EV_WARN, "Sockopt failure: %s",strerror(errno));
		wlogev(EV_WARN, "This is not critical, as JASM can work as usual");
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl (INADDR_ANY);
	server_address.sin_port = htons (SERVER_PORT);
	server_len = sizeof (server_address);

	if (bind (server_sockfd, (struct sockaddr *) &server_address, (socklen_t)server_len) < 0) {
		wlogev(EV_ERROR, "Socket binding failure: %s",strerror(errno));
		openlog ("JASM", LOG_PID, LOG_DAEMON);
		syslog (LOG_ERR, "socket not correctly binded... exiting!");
		closelog();
		exit (SOCKET_BINDING_FAILED);
	}

	if (listen (server_sockfd, 5) < 0) {
		wlogev(EV_ERROR, "Socket listening failure: %s",strerror(errno));
		openlog ("JASM", LOG_PID, LOG_DAEMON);
		syslog (LOG_ERR, "FATAL! socket was not put to listening mode! Exiting...");
		closelog();
		exit (SOCKET_LISTENING_CONNECTION_FAILED);
	}

	FD_ZERO (&readfds);
	FD_SET (server_sockfd, &readfds);

	wlogev(EV_INFO,"JASM is now ready");

	while (1) {
		char received[BUFSIZ];
		int fd,nread=-1;
		ssize_t rcval=-1;

		testfds = readfds;

		result = select(FD_SETSIZE, &testfds, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

		if (result < 1) {
			wlogev(EV_ERROR,"Select failure: %s",strerror(errno));
			exit(SOCKET_SELECT_FAILED);
		}

		for (fd = 0; fd < FD_SETSIZE; ++fd) {
			if (FD_ISSET (fd, &testfds)) {
				if (fd == server_sockfd) {
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address,
					                       &client_len);
					if (client_sockfd < 0) {
						wlogev(EV_ERROR, "Accepting new connection failure: %s", strerror(errno));
						wlogev(EV_WARN,"Ignored");
                        continue;
					}
					
					FD_SET (client_sockfd, &readfds);
					snprintf(client_ipaddr, 30,"%d.%d.%d.%d", \
					         client_address.sin_addr.s_addr & 0xFF, \
					         (client_address.sin_addr.s_addr & 0xFF00) >> 8, \
					         (client_address.sin_addr.s_addr & 0xFF0000) >> 16, \
					         (client_address.sin_addr.s_addr & 0xFF000000) >> 24);

					wlogev(EV_INFO, "Incoming connection - IP Address: %s",client_ipaddr);
					//handshake here

				} else {
					ioctl(fd, FIONREAD, &nread);

					if (nread == 0) {
						close(fd);
						FD_CLR (fd, &readfds);
					} else {
                        if((rcval = jasm_read(fd, received)) > 0)
                            execute_command(fd,received);
                        else 
                            jasm_write(fd,"CannotExecuteOperation",DATA_TEXT);
                    }
				}
			}
		}
	}
}

static inline ssize_t get_data_size(const char* data)
{
    char datacpy[MAX_HEADER_SIZE];
    strncpy(datacpy,data,MAX_HEADER_SIZE);
    
    char* current = strtok(datacpy,"\n");
    if(!current)
        return -2;
    
    while(current) {
        if(strstr(current,"Data-Size:")) {
            strtok(current," "); 
            return atol(strtok(NULL," "));
        }
        
        current = strtok(NULL,"\n");
    }
    
    return -3;
}

static ssize_t read_from_fd(int sockfd, char *__dest)
{
	if(sockfd < 0) {
		wlogev(EV_ERROR, "Invalid file descriptor");
		return -3;
	}

	ssize_t rcval;
	char __pre_dest[MAX_LENGHT_RECV];

	memset(__pre_dest,'\0',strlen(__pre_dest));
	if((rcval=recv(sockfd,__pre_dest,sizeof(__pre_dest),0)) == -1) {
		/* OVERFLOW PROTECTION */
		if(strlen(__pre_dest)+1 > MAX_LENGHT_RECV) {
			memset(__dest,'\0',strlen(__dest));
			return -2;
		}
		memset(__dest,'\0',strlen(__dest));
		wlogev(EV_ERROR, "Receiving from client failure: %s",strerror(errno));

		return -1;
	} else if (rcval == 0) {
		shutdown(sockfd,2);
		return 0;
	} else {
		memset(__dest,'\0',strlen(__dest));
		strncpy(__dest,__pre_dest,sizeof(__pre_dest));
		return rcval;
	}
}

static ssize_t write_to_fd(int sockfd, const char *__src)
{
	if (sockfd < 0) {
		wlogev(EV_ERROR, "Invalid file descriptor");
		return -3;
	}

	ssize_t rcval;
	char __final_src[MAX_LENGHT_SEND];

	/* OVERFLOW PROTECTION */
	if(strlen(__src)+1 > MAX_LENGHT_SEND) {
		if((rcval=send(sockfd,"TooLargeString",15,0)) == -1) {
			wlogev(EV_ERROR, "Sending to client failure: %s",strerror(errno));
		} else if (rcval == 0) {
			shutdown(sockfd,2);
		}
		return -2;
	}

	memset(__final_src,'\0',strlen(__final_src));
	strncpy(__final_src,__src,strlen(__src)+1);
	if((rcval=send(sockfd,__final_src,strlen(__final_src)+1,0)) == -1) {
		wlogev(EV_ERROR, "Sending to client failure: %s",strerror(errno));
		return -1;
	} else if (rcval == 0) {
		shutdown(sockfd,2);
		return 0;
	} else {
		return rcval;
	}
}

static inline ssize_t jasm_write(int sockfd, const char* __src, const char* data_type)
{
    if(!__src || !strcmp(data_type,DATA_NODATA))
        return -6;
    
    char final[MAX_LENGHT_SEND];
    snprintf(final,MAX_LENGHT_SEND,"Data-Type: %s\nData-Size: %ld\n\n%s",data_type,strlen(__src),__src);
	return write_to_fd(sockfd, final);
}

static inline ssize_t jasm_read(int sockfd, char* body)
{
    size_t i;
    const ssize_t rv = read_from_fd(sockfd,body);
    if(rv < 0 || rv == 0)
        return rv;
    
    char *pseparator = strstr(body, "\n\n");
    if(!pseparator)
        return -9;
    
    const size_t loc = strlen(body) - strlen(pseparator);
    
    char header[MAX_HEADER_SIZE];
    for(i=0;i<loc;i++) 
        header[i] = body[i];
    header[loc] = '\n';

    const ssize_t data_size = get_data_size(header);
    
    if (data_size <= 0)
        return -8;
    
    for(i=0;i<strlen(pseparator);i++)
        pseparator[i] = pseparator[i+2];

    if((ssize_t) strlen(pseparator) == data_size) 
        strncpy(body,pseparator,MAX_DATA_SIZE);
    else
        return -7;
    
    return data_size;
}

static inline ssize_t jasm_read_with_header(int sockfd, char* body, char* header)
{
    
}

static inline ssize_t jasm_write_with_header(int sockfd, const char* __src, const char* data_type, const char* header)
{
    if(!header)
        return -9;
    
    char final[MAX_LENGHT_SEND];
    size_t last_hdr = strlen(header)-1;
    
    if(header[last_hdr] != '\n')
        return -8;
        
    if(!strcmp(data_type,DATA_NODATA)) 
        snprintf(final,MAX_LENGHT_SEND,"Data-Type: %s\nData-Size: 0\n%s\n%s",DATA_NODATA,header,__src);
    else
        snprintf(final,MAX_LENGHT_SEND,"Data-Type: %s\nData-Size: %ld\n%s\n%s",data_type,strlen(__src),header,__src);
    
    return write_to_fd(sockfd, final);
}
