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
  int sockfd, portno;
  struct sockaddr_in serv_addr;
  struct hostent * server;
  int active = 1;
  
  char buffer[256];

  // Checking port and address as argument
  if (argc != 4) {
    error("Usage : client <portno> <host> <msg>");
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
  while(active) {

    memset(buffer, 0 , 256);
    //fgets(buffer, 255, stdin);
    
    send(sockfd, argv[3], 256, 0);
    recv(sockfd, buffer, 256, 0);
    printf("What the server got : %s\n", buffer);
    
    active = 0; //exit after the msg
  }
  return 0;
}
