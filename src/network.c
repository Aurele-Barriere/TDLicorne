#include "network.h"
#include "defines.h"

void send_verif(int sockfd, char * msg) {
  int sent = 0;
  int n = 0;
  int tries = 0;
  while(sent < BUFFER_SIZE && tries < MAX_TRIES) {
    tries += 1;
    n = send(sockfd, msg, BUFFER_SIZE, 0);
    sent += n;
  }
  if (tries >= MAX_TRIES) {
    error("Data not sent");
  }
}

void recv_verif(int sockfd, char * buffer) {
  int received = 0;
  int n = 0;
  int tries = 0;
  while(received < BUFFER_SIZE && tries < MAX_TRIES) {
    tries += 1;
    n = recv(sockfd, buffer, BUFFER_SIZE, 0);
    received += n;
  }
  if (tries >= MAX_TRIES) {
    error("Data not received");
  }
}

void send_to_both(char * msg, int sockfd1, int sockfd2) {
  send_verif(sockfd1, msg);
  send_verif(sockfd2, msg);
}

void error (char * msg) {
  printf("\n error : ");
  printf("%s",msg);
  printf("\n");
  exit(1);
}
