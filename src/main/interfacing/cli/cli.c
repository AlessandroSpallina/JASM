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
#include <pthread.h>

#include "ipc.h"
#include "miscellaneous.h"
#include "signals.h"

char * server_ip="127.0.0.1";
char name_temp[BUFSIZ];

int fd_table[NFDTABLE]={-1};

void add_fdtable(int newfd)
{
								for(int i=0; i<NFDTABLE; i++) {
																if(fd_table[i] == -1) {
																								fd_table[i] = newfd;
																								return;
																}
								}
}


void * async_read_socket(void *fd)
{
								ssize_t size;
								char buf[BUFSIZ];
								char filename[BUFSIZ];
								FILE *fp = NULL;
								int filedesc=(int *)fd;

								while(1) {
																if(read(filedesc, &size, sizeof(size)) == 0) {
																								fprintf(stderr, "Server Disconnected\n");
																								return NULL;
																}
																memset(buf, 0, BUFSIZ);
																if(read(filedesc, buf, size) == 0) {
																								fprintf(stderr, "Server Disconnected\n");
																								return NULL;
																}

																sprintf(filename, "../data/jasmcli.%s.module.output", name_temp);

																if((fp = fopen(filename, "a+")) != NULL)
																{
																								fprintf(fp, "%s\n", buf);
																								fclose(fp);
																} else {
																								fprintf(stderr, "Unable to open <%s>\n", filename);
																								return NULL;
																}

								}
}

void print_welcome(const char* usern, int sockfd,const char* releasetime, const char* debugrel)
{
		#ifdef CHECK_ACCESS_FILE
								//use it
		#else
								char CHECK_ACCESS_FILE[256];
								strcpy(CHECK_ACCESS_FILE, getenv("HOME"));
								strcat(CHECK_ACCESS_FILE, "/.jpwdchk");
		#endif // CHECK_ACCESS_FILE

								if(check_if_file_exists(CHECK_ACCESS_FILE) == 1) {
																char msg_from_file[BUFSIZ];
																FILE * fn;
																if((fn = fopen(CHECK_ACCESS_FILE,"r")) != NULL) {
																								fgets(msg_from_file,BUFSIZ,fn);
																								if(strcmp(msg_from_file, "true")==0 || strcmp(msg_from_file, "falset") == 0) {
																																printf("*IMPORTANT NOTICE*\n*JASM detected that someone tried to access this server more than 3 times!\n*Check server log!\n");
																																fclose(fn);
																																if((fn=fopen(CHECK_ACCESS_FILE,"w"))==NULL)
																																{ /*Stefano Belli Re dei Bug*/}
																																fprintf(fn,"false");
																																fclose(fn);
																								} else if(strcmp(msg_from_file,"false")==0)
																																fclose(fn);
																}
								}
								else
								{ /*Sei proprio uno sporcaccione*/}

								printf("%s\n", debugrel);
								printf("JASM Command Line Interface\nBuild date: %s\nSession started: %s\nSocket fd: %d\nUser: %s\nBasic Commands:\nhelp : get help\
  \nquit : exits cli\nhalt : halt jasm\n\n",releasetime,getTime(),sockfd,usern);
}

void secureJasmCommunication(char buffer[BUFSIZ], int fd)
{
								int n;
								int counter;

								if(strcmp("help", buffer)==0) {
																int ngetter = 0;
																int nmodule = 0;

																printf("Sending [%s]\n\n", buffer);
																if(write(fd, "help", strlen("help"))<0) {
																								perror("write on fd FAIL");
																}

																//gets getter list
																if((n = read(fd, &ngetter, sizeof(ngetter)))<0) {
																								perror("read on fd FAIL [helper getters]");
																} else if(n == 0) {
																				#ifdef DEBUG
																								printf("[DEBUG]Errno: %s",strerror(errno));
																				#endif // DEBUG

																								printf("* Server disconnected\n");
																								exit(SERVER_DISCONNECTED);
																}

																printf("\n**JASM Help page**\n");
																printf("* Getters *\n");
																char temp[BUFSIZ];

																for(int i=0; i<ngetter; i++) {
																								memset(temp, 0, BUFSIZ);
																								read(fd, &counter, sizeof(counter));
																								read(fd, temp, counter);

																								printf("%d# get%s\n", i, temp);
																}

																//gets module list
																if((n =  read(fd, &nmodule, sizeof(nmodule)))<0) {
																								perror("read on fd FAIL [helper modules]");
																} else if(n == 0) {
																				#ifdef DEBUG
																								printf("[DEBUG]Errno: %s",strerror(errno));
																				#endif // DEBUG

																								printf("* Server disconnected\n");
																								exit(SERVER_DISCONNECTED);

																}

																printf("\n* Modules * (star/stop)\n");
																for(int i=0; i<nmodule; i++) {
																								memset(temp, 0, BUFSIZ);
																								read(fd, &counter, sizeof(counter));
																								read(fd, temp, counter);

																								printf("%d# %s\n", i, temp);

																}
																//riceve altro
																return;
								}

								if(strncmp("start", buffer, 5)==0) {

																int fd_new = start_client(server_ip);
																if(write(fd_new, buffer, strlen(buffer)) < 0)
																{
																  fprintf(stderr,"* Failed to send start\n");
																	#ifdef DEBUG
																	fprintf(stderr,"[DEBUG]* Caused by: %s",strerror(errno));
																	#endif
																	exit(SOCKET_WRITE_FAILED);
																}

																strcpy(name_temp, &buffer[5]);
																memset(buffer, 0, BUFSIZ);

																if(read(fd_new, buffer, BUFSIZ) < 0)
																{
																	fprintf(stderr,"* Failed to read from server\n");
																	#ifdef DEBUG
																	fprintf(stderr,"[DEBUG]* Caused by: %s",strerror(errno));
																	#endif
																	exit(SOCKET_READ_FAILED);
																}

																#ifdef DEBUG
																printf("[DEBUG] Response: %s\n",buffer);
																#endif

																if(strcmp(buffer, "success") == 0) {
																								pthread_t tid;
																								add_fdtable(fd_new);
																								if(pthread_create(&tid, NULL, async_read_socket, (void *)fd_new) != 0) {
																																fprintf(stderr, "[ERROR] Fail to create a new thread: %s\n", strerror(errno));
																																return;
																								}
																								else
																								{
																									#ifdef DEBUG
																									printf("* Creating new thread success!\n");
																									#endif
																								}

																} else if (strcmp(buffer,"ModNotFound") == 0){
																								fprintf(stderr, "*[ERROR] Module Not found! Retry! \n");
																								close(fd_new);
																								return;
																} else {
																	              fprintf(stderr,"* Non-valid answer from server\n");
																								close(fd_new);
																								return;
																}
								} else {

																printf("sending [%s - %d byte]\n", buffer, (int) write(fd, buffer, strlen(buffer)));
																memset(buffer, 0, BUFSIZ);
																n = read(fd, buffer, BUFSIZ);
																if(n < 0) {
																								perror("* Error on read()");
																										#ifdef DEBUG
																								printf("[DEBUG] Errno result: %s\n", strerror(errno));
																										#endif
																} else if(n == 0) {
																												#ifdef DEBUG
																								printf("[DEBUG] Errno result: %s\n",strerror(errno));
																												#endif // DEBUG
																								printf("* Server disconnected\n");
																								exit(SERVER_DISCONNECTED);
																}

																printf("receiving [%s - %d byte]\n", buffer, n);
																return;
								}

}

void print_usage()
{
								printf("* Usage: jasmcli (just calls localhost)");
								printf("         jasmcli --connect-server <IPADDR> (calls IPADDR)");
}

void parse_options(int argc, char *argv[])
{
								if(argc > 1 && argc <= 3) {

																if(strcmp(argv[1],"--connect-server") == 0 ) {
																								if(argc == 3) {
																																server_ip = argv[2];
																																printf("* Connecting to: %s ...\n",server_ip);
																								} else if(argc < 3) {
																																printf("* You must specify an IP address!\n");
																																exit(ARG_SPECIFY_IPADDR);
																								}
																} else {
																								print_usage();
																								exit(0);
																}
								} else {
																if(argc >3) {
																								print_usage();
																								exit(0);
																}

								}
}

int main(int argc, char *argv[])
{
								check_debug();
								check_release();

								char buf[BUFSIZ];
								int fd;
								char *username = getenv("USER");

								parse_options(argc, argv);

								fd = start_client(server_ip);
								add_fdtable(fd);

								print_welcome(username, fd, buildtime, debugstr);

								signal_catcher();


								while(1) {
																printf("-[%s]-> ", username);
																scanf("%s", buf);
																if(strcmp(buf, "quit")==0) {
																								for(int z=0; z<NFDTABLE; z++)
																									if(fd_table[z] != 0)
																										close(fd_table[z]);
																								printf("Bye!\n");
																								exit(_EXIT_SUCCESS);
																} else {
																								secureJasmCommunication(buf, fd);
																}
																printf("\n");
								}
}
