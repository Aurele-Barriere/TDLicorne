#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "network.h"
#include "defines.h"

#include "board.h"

void game(int you, int sockfd) {
  int keep_playing = 1;
  int input;
  char buffer [BUFFER_SIZE];
  int player;
  int gamestate;
  int choice;
  int i;
  while(keep_playing) {
    //receiving gamestate and player
    memset(buffer, 0, BUFFER_SIZE);
    recv_verif(sockfd, buffer);
    
    gamestate = buffer[0];
    player = buffer[1];

    //checking for the end of the game
    if (gamestate == '\0') {
      printf("The game is over\n");
      keep_playing = 0;
      if (player == you) {
	printf(" You Won ! \n");
      }
      else {
	printf (" You lost \n");
      }
      break;
    }

    //rendering
    printf("\n");
    for (i=0; i<gamestate; i++) {
      printf(" | ");
    }
    printf("\n");

    // asking for input if needed
    if (player == you) {
      printf("This is your turn !\nHow much do you want to take? (must be between 1-3)\n");
      choice = getchar();
      getchar();
      choice -= '0';
      memset(buffer, 0, BUFFER_SIZE);
      buffer[0] = choice;
      printf("Your choice is %d\n", choice);
      send_verif(sockfd, buffer);
    }
    else {
      printf("Your opponent is playing, please wait...\n");
    }
	    
    
  }
}


int main(int argc, char * argv[]) {
  int sockfd, portno;
  struct sockaddr_in serv_addr;
  struct hostent * server;
  int active = 1;
  int i;
  char player;
  
  char buffer[BUFFER_SIZE];

  // Checking port and address as argument
  if (argc != 3) {
    error("Usage : client <portno> <host>");
  }

  portno = atoi(argv[1]);

  // checking host (?)
 // if (server == NULL) { error(" no nuch server ");}



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
  player = buffer[0] - '0';
  printf("You are player number %d\n", player);
  if (player == 1) {
    printf("Waiting for other player...\n");
  }
  
  memset(buffer, 0, BUFFER_SIZE);
  recv_verif(sockfd, buffer);
  
  for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++)
      board[i] = buffer[i];
  print_board(board);
    
  game(player, sockfd);

  close(sockfd);
  return 0;
}
