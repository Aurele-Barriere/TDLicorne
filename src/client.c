#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "network.h"
#include "defines.h"
#include "render.h"
#include "strategy.h"

#include "board.h"

void game_7colors(char you, int sockfd) {
  printf("Beginning of game of 7 colors\n");
  
  printf("Your color is %c\n", you);
  
  bool keep_playing = TRUE;
  char choice;
  char buffer [BUFFER_SIZE];
  int player;
  int i;
  
  
  
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("Les 7 merveilles du monde des 7 couleurs",SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    600,
                                    600,
                                    SDL_WINDOW_SHOWN);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  SDL_Event event;
    
    
  while(keep_playing) 
  {
      
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                keep_playing = FALSE;
                break;
                
        }
    }
    
    
    //receiving game state and player
    memset(buffer, 0, BUFFER_SIZE);
    recv_verif(sockfd, buffer);
    for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
      board[i] = buffer[i+1];
    }
    player = buffer[0];
    
    //checking for end condition
    if (buffer[0] == '*') {
      keep_playing = 0;
      player = buffer[1];
      if (player == 0) {
	printf("Draw !");
      }
      else if (player == you) {
	printf("You Won !\n");
      }
      else {
	printf("You Lose !\n");
      }
      break;
    }
    
    //rendering
    //print_board(board);
    display_board(renderer, board);
    
    //asking for input if needed
    if (player == you) 
    {
      choice = player_choice(you) + 'a';
      memset(buffer, 0, BUFFER_SIZE);
      buffer[0] = choice;
      printf("Your choice is %c\n", choice);
      send_verif(sockfd, buffer);
    }
    else {
      printf("Your opponent is playing, please wait...\n");
    }    
  }
  
    SDL_DestroyWindow(window);
    SDL_Quit();
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
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error(" connecting ");
    
    
    return sockfd;
}

int main(int argc, char * argv[]) {
  int sockfd;
  int i;
  char player;
  
  char buffer[BUFFER_SIZE];

  // Checking port and address as argument
  if (argc != 3) {
    error("Usage : client <portno> <host>");
  }
  
  // Creating socket
  sockfd = init_client(argv[1], argv[2]);
  
  // sending / recieving

  memset(buffer, 0 , BUFFER_SIZE);
  recv_verif(sockfd, buffer);
  player = buffer[0];
  printf("You are player %c\n", player);
  printf("Waiting for other player...\n");
  
  memset(buffer, 0, BUFFER_SIZE);
  
  for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++)
      board[i] = buffer[i];
    
  game_7colors(player, sockfd);

  close(sockfd);
  return 0;
}
