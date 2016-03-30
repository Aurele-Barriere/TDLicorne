#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "network.h"
#include "defines.h"

#include "board.h"

void game_7colors(int you, int sockfd) {
  printf("Beginning of game of 7 colors\n");
  if (you == 1) {
    printf("Your color is %c\n", color1+97);
  }
  else {
    printf("Your color is %c\n", color2+97);
  }
  int keep_playing = 1;
  char choice;
  char buffer [BUFFER_SIZE];
  int player;
  int i;
  while(keep_playing) {
    //receiving game state and player
    memset(buffer, 0, BUFFER_SIZE);
    recv_verif(sockfd, buffer);
    for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
      board[i] = buffer[i];
    }
    player = buffer[BOARD_SIZE*BOARD_SIZE];
    
    //checking for end condition
    //TO DO
    
    //rendering
    print_board(board);
    
    //asking for input if needed
    if (player == you) {
      printf("This is your turn !\nWhat is your color?\n");
      choice = getchar();
      getchar();
      memset(buffer, 0, BUFFER_SIZE);
      buffer[0] = choice;
      printf("Your choice is %c\n", choice);
      send_verif(sockfd, buffer);
    }
    else {
      printf("Your opponent is playing, please wait...\n");
    }    
  }
}

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

int init_client(const char* portno, const char* addr)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    
    if (sockfd < 0) 
        error("creating socket");

    // Configure serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(atoi(portno));

    //connecting
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error(" connecting ");
    }
    
    return sockfd;
}

int main(int argc, char * argv[]) {
  int sockfd;
  //struct hostent * server;
  //int active = 1;
  int i;
  char player;
  
  char buffer[BUFFER_SIZE];

  // Checking port and address as argument
  if (argc != 3) {
    error("Usage : client <portno> <host>");
  }


  // checking host (?)
 // if (server == NULL) { error(" no nuch server ");}



  // Creating socket
  sockfd = init_client(argv[1], argv[2]);
  

  // sending / recieving

  memset(buffer, 0 , BUFFER_SIZE);
  recv_verif(sockfd, buffer);
  player = buffer[0] - '0';
  printf("You are player number %d\n", player);
  if (player == 1) {
    printf("Waiting for other player...\n");
  }
  
  memset(buffer, 0, BUFFER_SIZE);
  //recv_verif(sockfd, buffer);
  
  for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++)
      board[i] = buffer[i];
    
  game_7colors(player, sockfd);

  close(sockfd);
  return 0;
}
