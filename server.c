#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

void game(int sockfd1, int sockfd2);


void send_to_both(char * msg, int sockfd1, int sockfd2) {
  send(sockfd1, msg, BUFFER_SIZE, 0);
  send(sockfd2, msg, BUFFER_SIZE, 0);
}

void error (char * msg) {
  printf("\n error : ");
  printf("%s",msg);
  printf("\n");
  exit(1);
}


void send_verif(int sockfd, char * msg) {
  int sent = 0;
  int n = 0;
  while(sent < BUFFER_SIZE) {
    n = send(sockfd, msg, BUFFER_SIZE, 0);
    sent += n;
  }
}

void recv_verif(int sockfd, char * buffer) {
  int received = 0;
  int n = 0;
  while(received < BUFFER_SIZE) {
    n = recv(sockfd, buffer, BUFFER_SIZE, 0);
    received += n;
  }
}

int main(int argc, char * argv[]) {
  int sockfd, sockfd1, sockfd2, portno;
  socklen_t clilen1, clilen2;
  struct sockaddr_in cli1, cli2, serv_addr;
  char buffer[BUFFER_SIZE];
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

  // waiting for clients to connect
  sockfd1 = accept(sockfd, (struct sockaddr *) &cli1, &clilen1);
  // player 1 is here
  memset(buffer, 0, BUFFER_SIZE);
  send(sockfd1, "1", BUFFER_SIZE, 0); // sending player id
  sockfd2 = accept(sockfd, (struct sockaddr *) &cli2, &clilen2);
  //player 2 is here
  send(sockfd2, "2", BUFFER_SIZE, 0); // sending player id
  
  //all players are connected, strating game
  game(sockfd1, sockfd2);

  //once everything is done
  printf("\n Server shutdown \n");
  close(sockfd);
  close(sockfd1);
  close(sockfd2);
 return 0;
}


void game(int sockfd1, int sockfd2) {
  char msg [] = "The game is about to start\n";
  send_to_both(msg, sockfd1, sockfd2);
  

}
