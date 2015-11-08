#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ipc.h"

void print_menu()
{
  printf("1) get command list\na) send command\n0) exit\n");
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
      case '1':

        //write(fd, (void *)bufstr, sizeof(bufstr));
        break;

      case 'a':
        printf("type the command: ");
        scanf("%s", bufstr);
        write(fd, (void *)bufstr, sizeof(bufstr));
        read(fd, (void *)bufstr, sizeof(bufstr));
        printf("@ received: %s\n", bufstr);
        break;

      case '0':
        close(fd);
        printf("bye!\n");
        exit(0);
        break;
    }
  }


}
