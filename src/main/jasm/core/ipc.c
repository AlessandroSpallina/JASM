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
#include "ipc.h"
#include "miscellaneous.h"


static void excecute_command(int fd, const char *command)
{
  if(strcmp("getVersion", command)==0) {
    write(fd, (void *)VERSION, sizeof(VERSION));
    log_string("server reply <version> with success");
    return;
  }

  log_error("command received not found");
  write(fd, (void *)"null", 4);
}

void start_server()
{
int server_sockfd, client_sockfd;
int server_len;
socklen_t client_len;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
int result;
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
  
  log_string("server started");

  while(1) {
    char buf[BUFSIZ];
    char received[BUFSIZ];
    int fd;
    int nread;

    testfds=readfds;

    result=select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);

    if(result<1) {
      log_error("server fail");
      exit(1);
    }

    for(fd=0; fd<FD_SETSIZE; fd++) {
      if(FD_ISSET(fd, &testfds)) {
        if(fd==server_sockfd) {
          client_len=sizeof(client_address);
          client_sockfd=accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
          FD_SET(client_sockfd, &readfds);
          sprintf(buf, "adding client on fd %d", client_sockfd);
          log_string(buf);
        } else {
          ioctl(fd, FIONREAD, &nread);

          if(nread==0) {
            close(fd);
            FD_CLR(fd, &readfds);
            sprintf(buf, "removing client on fd %d", fd);
            log_string(buf);
          } else {
            read(fd, &received, BUFSIZ);
            sprintf(buf, "received from fd %d command <%s>", fd, received);
            log_string(buf);
            excecute_command(fd, received);
          }
        }
      }
    }
  }
}
