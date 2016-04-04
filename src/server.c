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

void game_7colors(int sockfd);


// à compléter !!!
int game_over(char* board, struct client_set* player)
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
    //when a player disconnects, her opponent wins
    if (player->nb >= 2) {
      /* if (player->is_connected[0] == 0)
	return color2 + 97;
      if (player->is_connected[1] == 0)
      return color1 + 97;*/
    }
    return 0;
}





int main(int argc, char * argv[]) {
  int sockfd;

  // Checking port as argument
  if (argc != 2) 
      error("Usage : server <portno>");
  


  // Creating socket
  sockfd = init_server(argv[1]);
  
  // listening
  listen(sockfd, 42);
  
  // strating the game
  game_7colors(sockfd);

  //once everything is done
  printf("\nServer shutdown \n");
  close(sockfd);
  return 0;
}

void game_7colors(int sockfd)
{
    srand(time(NULL)); //initializing random
    // the 7 colors game
    printf("Starting game of 7 colors\n\n");
    int winner = 0;
    char choice;
    char buffer [BUFFER_SIZE];
    int i;
    
    int sock;
    
    const unsigned MAX_PLAYER = 2;
    
    
    struct client_set player = client_set_init();
    struct client_set obs = client_set_init();
    
    char symbols[] = {'^', '@'};
    int current_player = rand() % 2;
    
    bool send_board = FALSE;

    
    set_sym_board(); //initializing board
    printf("The board has been generated.\n\n");
    
    printf("Waiting for players to connect.\n");
    printf("%d more players required !\n\n", MAX_PLAYER - player.nb);
    
    while(!(winner = game_over(board, &player))) 
    {       
    
        // if a client/observer tries to connect    
        if(socket_ready(sockfd, 50))
        {
            sock = wait_client(sockfd);
            
            memset(buffer, 0, BUFFER_SIZE);
            recv_verif(sock, buffer);
            
            
            // buffer[0] contains the type of client :
            switch (buffer[0])
            {
                case 'p': // player
                    printf("A new player tried to connect\n");
                    if (player.nb < MAX_PLAYER)
                    {
                        client_set_add(&player, sock);
                        
                        send_char(sock, symbols[player.nb-1]);
                        printf("Player %c successfully connected !\n", symbols[player.nb-1]);  
                        
                        if (player.nb == MAX_PLAYER)
                        {
                            printf("All the player are connected... Let's start the game !\n\n");
                            send_board = TRUE;
                        }
                        else
                        {
                            printf("%d more players required !\n\n", MAX_PLAYER - player.nb);
                        }
                    }
                    else
                    {
                        printf("Cannot accept any more client\n\n");
                    }

                    break;
                case 'o': // observer
                    printf("A new observer tried to connect\n");     
                    client_set_add(&obs, sock);
                    printf("Observer successfully connected !\n\n");            
                    break;
                default:
                    printf("An unknown client has been rejected\n\n");
                    close(sock);
                    break;
            }
        }
        
        
        //sending board and current player
        if (send_board)
        {
            memset(buffer, 0, BUFFER_SIZE);
            
            buffer[0] = symbols[current_player];
            for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) 
                buffer[i+1] = board[i];
            
            
            client_set_send(player, buffer);
            client_set_send(obs, buffer);
            
            send_board = FALSE;
        }
        

        //awaiting input from player
        if(player.nb == MAX_PLAYER && socket_ready(player.sockfd[current_player], 50))
        {
            memset(buffer, 0, BUFFER_SIZE);
            recv_verif(player.sockfd[current_player], buffer);
            
            
            
            choice = buffer[0];
            printf("Player %c played color %c\n", symbols[current_player], choice);
            //rule checking
            if (choice < 'a' || choice >= 'a' + NB_COLORS) 
            {
                choice = rand() % NB_COLORS;
                choice += 'a';
                printf("Wrong input, player has been assigned color %c\n", choice);
            }
            
            //updating game state
            update_board(symbols[current_player] - 'a', choice - 'a', board);
            
            
            //changing player
            current_player = (current_player + 1) % player.nb;
            
            send_board = TRUE;
        }
    }
  
  
    // end of game
  
    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = '*';
    buffer[1] = winner;
    
    printf("Game over !\n");
    if (winner == -1)
        printf("Draw\n");
    else
        printf("Player %c won !\n", winner);
  

    client_set_send(player, buffer);
    
    
    client_set_close(player);
    client_set_close(obs);
}
