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


void game_7colors(char you, int sockfd) 
{
    printf("Beginning of game of 7 colors\n");
    
    printf("Your color is %c\n", you);
    
    char choice;
    char buffer [BUFFER_SIZE];
    int player = 0;
    int winner = 0;
    
    
    memset(buffer, 0, BUFFER_SIZE);
    
    
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("7 colors : player", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        WINDOW_WIDTH,
                                        WINDOW_HEIGHT,
                                        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Event event;
        
        
    while(winner == 0) 
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    winner = -1;
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if (player == you) //asking for input if needed
                    {
                        choice = get_cell(event.motion.x / (WINDOW_WIDTH / BOARD_SIZE),
                                          event.motion.y / (WINDOW_HEIGHT / BOARD_SIZE),
                                          buffer+1) + 'a';
                        memset(buffer, 0, BUFFER_SIZE);
                        buffer[0] = choice;
                        send_verif(sockfd, buffer);
                        player = 0;
                    }
                default:
                    break;
            }
        }
    
    
        //receiving game state and player    
        if(socket_ready(sockfd, 50)) // non-blocking
        {
            memset(buffer, 0 , BUFFER_SIZE);
            recv_verif(sockfd, buffer);
            display_board(renderer, buffer+1);
            
            player = buffer[0];
            //checking for end condition :
            // if buffer[0] == '*', the game is over, and buffer[1] contains the winner
            if (buffer[0] == '*') 
                winner = buffer[1];
            else if (player == you) //asking for input if needed
                printf("It is your turn !\n");
            else 
                printf("Your opponent is playing, please wait...\n");
        }
    }
  
    if (winner == -1) 
        printf("Draw !\n");
    else if (winner == you) 
        printf("You Won !\n");
    else 
        printf("You Lose !\n");
  
    SDL_Delay(1000);
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}



int main(int argc, char * argv[]) {
  int sockfd;
  char player;
  
  char buffer[BUFFER_SIZE];

  // Checking port and address as argument
  if (argc != 3) {
    error("Usage : client <portno> <host>");
  }
  
  // Creating socket
  sockfd = init_client(argv[1], argv[2], 'p');
  
  // sending / recieving

  memset(buffer, 0 , BUFFER_SIZE);
  recv_verif(sockfd, buffer);
  player = buffer[0];
  printf("You are player %c\n", player);
  printf("Waiting for other player...\n");
  
  
    
  game_7colors(player, sockfd);

  close(sockfd);
  return 0;
}
