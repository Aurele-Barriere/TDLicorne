#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "network.h"
#include "defines.h"

#include "board.h"


void game(int sockfd1, int sockfd2);
void game_7colors(int sockfd1, int sockfd2);


int init_server(const char* portno)
{
    struct sockaddr_in serv_addr;
    int sockfd;
 

    // Creating socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {error("creating socket");}

    // Configure serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(portno));

    // binding socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0) {
        error("binding");
    }
    
    return sockfd;
}

int wait_player(int sockfd, char id)
{
    socklen_t clilen;
    struct sockaddr_in cli;
    char buffer[BUFFER_SIZE];
        
    int sockfd1 = accept(sockfd, (struct sockaddr *) &cli, &clilen);
    // player 1 is here
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = id;
    send_verif(sockfd1, buffer);
    
    return sockfd1;
}


int main(int argc, char * argv[]) {
  int sockfd, sockfd1, sockfd2;
  char buffer[BUFFER_SIZE];
  int active = 1;

  // Checking port as argument
  if (argc != 2) {
    error("Usage : server <portno>");
  }


  // Creating socket
  sockfd = init_server(argv[1]);
  
  // listening
  listen(sockfd, 5);

  // waiting for clients to connect
  sockfd1 = wait_player(sockfd, '^');
  sockfd2 = wait_player(sockfd, '@');
  
  printf("both players connected\n");
  //all players are connected, starting game
  game_7colors(sockfd1, sockfd2);

  //once everything is done
  printf("\n Server shutdown \n");
  close(sockfd);
  close(sockfd1);
  close(sockfd2);
 return 0;
}

void next_player(char* player)
{
    *player = (color1 + 97) + (color2 + 97) - *player;
}

void game_7colors(int sockfd1, int sockfd2)
{
  // the 7 colors game
  printf("Starting game of 7 colors\n");
  int keep_playing = 1;
  char player = color1 + 97;
  int winner = 0;
  char choice;
  char buffer [BUFFER_SIZE];
  int score1 = 0;
  int score2 = 0;
  int i;
  

  srand(time(NULL)); //initializing random
  if (rand() % 2)
      next_player(&player);
  
  set_sym_board(); //initializing board

  printf("The board has been generated :\n");
  print_board(board);
  while(keep_playing) {
    //sending board and player
    memset(buffer, 0, BUFFER_SIZE);
    for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
      buffer[i+1] = board[i];
    }
    buffer[0] = player;
    send_to_both(buffer, sockfd1, sockfd2);

    //awaiting input from player
    memset(buffer, 0, BUFFER_SIZE);
    if (player == color1 + 97) {
      recv_verif(sockfd1, buffer);
    }
    else {
      recv_verif(sockfd2, buffer);
    }
    
    choice = buffer[0];
    printf("Player %c played color %c\n", player, choice);
    //rule checking
    if (choice < 'a' || choice >= 'a' + NB_COLORS) {
      choice = rand() % NB_COLORS;
      choice += 'a';
      printf("Wrong input, player has been assigned color %c\n", choice);
    }
    
    //updating game state
    update_board(player - 97, choice - 'a', board);
      
    //updating score
    score1 = score(board, color1);
    score2 = score(board, color2);

    //checking end game condition
    // TO DO

    //changing player
    next_player(&player);
  }
}
void game(int sockfd1, int sockfd2) {
  // for now, we implement a small version of the Marienbad game
  // for 7 colors, the steps should be the same
  // but we should encapsulate better game state

  send_to_both(board, sockfd1, sockfd2);
  int keep_playing = 1;
  int gamestate = 21;
  int player = 1;
  int winner = 0;
  int choice;
  char buffer [BUFFER_SIZE];
  while (keep_playing) {
    // sending gamestate and player
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = gamestate; // we will send a special character when the game is over
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
    if (choice > 3 || choice < 1 || choice > gamestate-1) { choice = 1; } 
    //rule checking must be done server-side 
    //we're never sure of what code the client is running
    gamestate -= choice;

    //checking end condition
    if (gamestate == 1) {
      winner = player;
      keep_playing = 0;
    }

    // changing player
    player = 3-player;
  }

  // sending the result at the end of the game
  memset(buffer, 0, BUFFER_SIZE);
  buffer[0] = '\0';
  buffer[1] = winner;
  send_to_both(buffer, sockfd1, sockfd2);
  
}