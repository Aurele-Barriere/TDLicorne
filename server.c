#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error (char * msg) {
  printf("\n error : ");
  printf("%s",msg);
  printf("\n");
  exit(1);
}

int main(int argc, char * argv[]) {
  int sockfd, acksockfd, portno;
  socklen_t clilen;
  struct sockaddr_in cli_addr, serv_addr;
  char buffer[256];
  int active = 1;

  // Checking port as argument
  if (argc != 2) {
    error("Usage : server <portno>");
  }

  portno = atoi(argv[1]);


  // Creating socket
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {error("creating socket");}

  // Configure serv_addr
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  


  // binding socket
  if (
      bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
      < 0) {
    error("binding");
  }
  
  // listening
  listen(sockfd, 5);

  // waiting for client to connect
  while (active) {
    acksockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    memset(buffer, 0, 256);
    
    recv(acksockfd, buffer, 256, 0);
    printf("%s\n", buffer);
    send(acksockfd, buffer, 256, 0);
  }

  
 
  printf("\n Server shutdown \n");
 return 0;
}


