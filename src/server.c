#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "network.h"
#include "defines.h"



void game(int sockfd1, int sockfd2);


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
  buffer[0] = '1';
  send_verif(sockfd1, buffer); // sending player id
  sockfd2 = accept(sockfd, (struct sockaddr *) &cli2, &clilen2);
  //player 2 is here
  memset(buffer, 0 , BUFFER_SIZE);
  buffer[0] = '2';
  send_verif(sockfd2, buffer); // sending player id
  
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
  // for now, we implement a small version of the Marienbad game
  // for 7 colors, the steps should be the same
  // but we should encapsulate better game state

  char msg [] = "The game is about to start\n";
  send_to_both(msg, sockfd1, sockfd2);
  int keep_playing = 1;
  int gamestate = 21;
  int player = 1;
  int choice;
  char buffer [BUFFER_SIZE];
  while (keep_playing) {
    // sending gamestate and player
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = gamestate;
    buffer[1] = player;
    send_to_both(buffer, sockfd1, sockfd2);

    // awaiting input from player
    memset(buffer, 0, BUFFER_SIZE);
    if (player == 1) {
      recv_verif(sockfd1, buffer);
    }
    else {
      recv_verif(sockfd2, buffer);
    }

    // updating game state
    choice = buffer[0];
    if (choice > 3 || choice < 1) { choice = 1; } //rule checking must be done server-side (prevent cheating)
    gamestate -= choice;

    // changing player
    player = 3-player;

    //checking end condition
    // TO DO
   
  
  }

}
