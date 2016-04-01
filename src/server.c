#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "network.h"
#include "defines.h"
#include "render.h"

#include "board.h"

void game_7colors(int sockfd, int* sockfd_cli);

int game_over(char* board)
{
    int score1 = score(board, color1);
    int score2 = score(board, color2);
    int limit = (BOARD_SIZE*BOARD_SIZE)/2;
    
    if (score1 > limit)
        return color1 + 97;
    if (score2 > limit)
        return color2 + 97;
    if (score1 == limit && score2 == limit)
        return -1;
    return 0;
}

/** Victory condition */
int victory(int score1, int score2) {
  int limit = (BOARD_SIZE*BOARD_SIZE)/2;
  if (score1>limit || score2>limit) {
    return 1;
  }
  return 0;
}


/** Draw condition */
int draw(int score1, int score2) {
  int limit = (BOARD_SIZE*BOARD_SIZE)/2;
  if (score1 == limit && score2 == limit) {
    return 1;
  }
  return 0;
}



int wait_client(int sockfd, char id)
{
    socklen_t clilen;
    struct sockaddr_in cli;
    char buffer[BUFFER_SIZE];
        
    int sockfd1 = accept(sockfd, (struct sockaddr *) &cli, &clilen);
    // player is here
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = id;
    send_verif(sockfd1, buffer);
    return sockfd1;
}



int main(int argc, char * argv[]) {
  int sockfd, sockfd_cli[2];

  // Checking port as argument
  if (argc != 2) {
    error("Usage : server <portno>");
  }


  // Creating socket
  sockfd = init_server(argv[1]);
  
  // listening
  listen(sockfd, 42);
  
  // waiting for clients to connect
  sockfd_cli[0] = wait_client(sockfd, '^');
  sockfd_cli[1] = wait_client(sockfd, '@');
  
  printf("both players connected\n");
  //all players are connected, starting game
  game_7colors(sockfd, sockfd_cli);

  //once everything is done
  printf("\n Server shutdown \n");
  close(sockfd);
  close(sockfd_cli[0]);
  close(sockfd_cli[1]);
 return 0;
}

void next_player(char* player)
{
    *player = (color1 + 97) + (color2 + 97) - *player;
}

void game_7colors(int sockfd, int* sockfd_cli)
{
  // the 7 colors game
  printf("Starting game of 7 colors\n");
  int winner = 0;
  char player = color1 + 97;
  char choice;
  char buffer [BUFFER_SIZE];
  int i;
  
  int* sockfd_obs = NULL;
  int nb_obs = 0;
  
  

  srand(time(NULL)); //initializing random
  if (rand() % 2)
      next_player(&player);
  
  set_sym_board(); //initializing board
  printf("The board has been generated.\n");
  
  while(!(winner = game_over(board))) {
      
    
    // observer connection    
    if(socket_ready(sockfd, 50))
    {
        nb_obs++;
        sockfd_obs = (int*) realloc (sockfd_obs, nb_obs * sizeof(int));
        sockfd_obs[nb_obs-1] = wait_client(sockfd, 'o');
        printf("An observer just connected\n");
    }
    
    //sending board and player
    memset(buffer, 0, BUFFER_SIZE);
    for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
      buffer[i+1] = board[i];
    }
    buffer[0] = player;
    
    send_to_all(buffer, sockfd_cli, 2);
    send_to_all(buffer, sockfd_obs, nb_obs);
    

    //awaiting input from player
    memset(buffer, 0, BUFFER_SIZE);
    if (player == color1 + 97) {
      recv_verif(sockfd_cli[0], buffer);
    }
    else {
      recv_verif(sockfd_cli[1], buffer);
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
    update_board(player - 'a', choice - 'a', board);
      
    
    
    
    //changing player
    next_player(&player);
  }
  
  
  
  
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = '*';
    buffer[1] = winner;
    
    printf("Game over !\n");
    if (winner == -1)
        printf("Draw\n");
    else
        printf("Player %c won !\n", winner);
  

    send_to_all(buffer, sockfd_cli, 2);
  
    for (i = 0; i < nb_obs; i++)
        close(sockfd_obs[i]);
    free(sockfd_obs);
}
