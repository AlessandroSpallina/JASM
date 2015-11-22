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
#include "miscellaneous.h"

void print_welcome()
{
  printf("Welcome!\nThis is a commandline interface of JASM\nType your command, 0 to exit..\n");
}

void secureJasmCommunication(char buffer[BUFSIZ], int fd)
{

	if(strcmp("0", buffer)==0) {
		close(fd);
		printf("bye!\n");
		exit(0);
	} else {
	
		if(strncmp("start", buffer, 5)==0) {
			//starting thread-module -> opening new dedicated socket
   		//so we can get different output from jasm thanks to fd.
   			
   			/*
   			 *	new socket
   			 *	write()
   			 *	read()
   			 */
   			return;
	
		} else {
		printf("sending [%s]\n", buffer);
    		write(fd, (void *)buffer, BUFSIZ);
    		memset(buffer, 0, BUFSIZ);
    		read(fd, (void *)buffer, BUFSIZ);
    		return;
		}
	
	}

}

int main(int argc, char *argv[])
{
  int fd;
  char buf[BUFSIZ]="none";

  fd=start_client();

  print_welcome();

  while(1) {
    printf("> ");
    scanf("%s", buf);
	secureJasmCommunication(buf, fd);
    printf("%s\n", buf);
    printf("\n");
  }
}
