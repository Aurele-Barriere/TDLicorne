#include "network.h"
#include "defines.h"

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

void send_to_both(char * msg, int sockfd1, int sockfd2) {
  send_verif(sockfd1, msg);
  send_verif(sockfd2, msg);
}
