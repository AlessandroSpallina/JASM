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

void print_menu()
{
  printf("a) send command\n0) exit\n");
}

int main(int argc, char *argv[])
{
  int fd;
  char buf;
  char bufstr[BUFSIZ];

  fd=start_client();

  while(1) {
    print_menu();
    scanf("%c", &buf);

    switch(buf) {

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
  }


}
