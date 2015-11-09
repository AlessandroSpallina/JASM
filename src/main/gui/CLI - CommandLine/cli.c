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

void parseCommand_sendJasm(const char *buffer, int fd)
{

	if(strcmp("0", buffer)==0) {
		close(fd);
		print("bye!\n");
		exit(0);
	} else {
	
		if(strncmp("start", buffer, 5)==0) {
			//sto startando un modulo-thread -> apro un nuovo socket dedicato
   			//in modo tale da avere diversi fd per diversi output da jasm
   			
   			/*
   			 *	new socket
   			 *	write()
   			 *	read()
   			 */
   			return;
	
		} else {
    		write(fd, (void *)buffer, sizeof(buffer));
    		memset(buf, 0, BUFSIZ);
    		read(fd, (void *)buffer, sizeof(buffer));
    		return;
		}
	
	}

}

int main(int argc, char *argv[])
{
  int fd;
  char buf[BUFSIZ];

  fd=start_client();

  print_welcome();

  while(1) {
    printf("> ");
    scanf("%s", buf);

    printf("%s\n", buf);
    printf("\n");
  }


  /*while(1) {
    print_menu();
    scanf("%c", &buf);

    switch(buf) {

      fflush(stdin);

      case 'a':
        printf("type the command: ");
        scanf("%s", bufstr);

        if(strncmp("start", bufstr, 5)==0) {
        	write(fd, (void *)bufstr, sizeof(bufstr));
        	read(fd, (void *)bufstr, sizeof(bufstr));
        	if(strcmp("success", bufstr)==0) {
        		//sto startando un modulo-thread -> apro un nuovo socket dedicato
        		//in modo tale da avere diversi fd per diversi output da jasm
        	} else {
        		fprintf(stderr, "start new module fail\n");
        	}


        } else {
          printf("stringa: %s\n", bufstr);
        	write(fd, (void *)bufstr, sizeof(bufstr));
        	read(fd, (void *)bufstr, sizeof(bufstr));
        	printf("@ received: %s\n", bufstr);
        }
        break;

      case '0':
        close(fd);
        printf("bye!\n");
        exit(0);
        break;

      default:
      	fprintf(stderr, "wrong option\n");
      	break;
    }
  }*/


}
