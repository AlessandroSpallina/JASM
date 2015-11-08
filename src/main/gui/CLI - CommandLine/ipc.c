#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "ipc.h"

/*
 *  Connette al server e ritorna il fd del socket relativo
 *  ricordati di chiudere il fd prima di chiudere il programma
 */
int start_client()
{
  int sockfd;
  int len;
  struct sockaddr_in address;
  int result;

  sockfd=socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family=AF_INET;
  address.sin_addr.s_addr=inet_addr(SERVER_IP);
  address.sin_port=htons(SERVER_PORT);
  len=sizeof(address);

  result=connect(sockfd, (struct sockaddr *)&address, len);

  if(result==-1) {
    fprintf(stderr, "unable to connect with server\n");
    exit(1);
  }

  return sockfd;
}
