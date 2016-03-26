#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "network.h"
#include "defines.h"


int main(int argc, char * argv[]) {
  int sockfd, portno;
  struct sockaddr_in serv_addr;
  struct hostent * server;
  int active = 1;
  char player;
  
  char buffer[BUFFER_SIZE];

  // Checking port and address as argument
  if (argc != 3) {
    error("Usage : client <portno> <host>");
  }

  portno = atoi(argv[1]);

  // checking host
  if (server == NULL) { error(" no nuch server ");}



  // Creating socket
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {error("creating socket");}

 // Configure serv_addr
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
  serv_addr.sin_port = htons(portno);

  //connecting
  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error(" connecting ");
  }

  // sending / recieving

  memset(buffer, 0 , BUFFER_SIZE);
  recv_verif(sockfd, buffer);
  player = buffer[0];
  printf("You are player number %s\n", buffer);
  //printf("%c\n", player);
  memset(buffer, 0, BUFFER_SIZE);
  recv_verif(sockfd, buffer);
  printf("%s", buffer);
    
    

    close(sockfd);
  return 0;
}
