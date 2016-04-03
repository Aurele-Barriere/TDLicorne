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


int wait_client(int sockfd)
{
    socklen_t clilen;
    struct sockaddr_in cli;
        
    int sockfd1 = accept(sockfd, (struct sockaddr *) &cli, &clilen);
    
        
    return sockfd1;
}

void add_client(int sockfd, int** sockfd_tab, int* nb)
{
    (*nb)++;
    (*sockfd_tab) = (int*) realloc (*sockfd_tab, (*nb) * sizeof(int));
    (*sockfd_tab)[(*nb)-1] = sockfd;
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
  //sockfd_cli[0] = wait_client(sockfd, '^');
  //sockfd_cli[1] = wait_client(sockfd, '@');
  
  //printf("both players connected\n");
  //all players are connected, starting game
  game_7colors(sockfd);

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
    
    int* sockfd_cli = NULL;
    int nb_cli = 0;
    const int MAX_PLAYER = 2;
    
    int* sockfd_obs = NULL;
    int nb_obs = 0;
    
    char player[] = {'^', '@'};
    int current_player = rand() % 2;
    
    bool send_board = FALSE;

    
    set_sym_board(); //initializing board
    printf("The board has been generated.\n\n");
    
    printf("Waiting for players to connect.\n");
    printf("%d more players required !\n\n", MAX_PLAYER - nb_cli);
    
    while(!(winner = game_over(board))) 
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
                    if (nb_cli < MAX_PLAYER)
                    {
                        /*
                        nb_cli++;
                        sockfd_cli = (int*) realloc (sockfd_cli, nb_cli * sizeof(int));
                        sockfd_cli[nb_cli-1] = sock;
                        */
                        add_client(sock, &sockfd_cli, &nb_cli);
                        
                        send_char(sock, player[nb_cli-1]);
                        printf("Player %c successfully connected !\n", player[nb_cli-1]);  
                        
                        if (nb_cli == MAX_PLAYER)
                        {
                            printf("All the player are connected... Let's start the game !\n\n");
                            send_board = TRUE;
                        }
                        else
                        {
                            printf("%d more players required !\n\n", MAX_PLAYER - nb_cli);
                        }
                    }
                    else
                    {
                        printf("Cannot accept any more client\n\n");
                    }

                    break;
                case 'o': // observer
                    printf("A new observer tried to connect\n");
                    /*
                    nb_obs++;
                    sockfd_obs = (int*) realloc (sockfd_obs, nb_obs * sizeof(int));
                    sockfd_obs[nb_obs-1] = sock;    
                    */
                
                    add_client(sock, &sockfd_obs, &nb_obs);
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
            
            buffer[0] = player[current_player];
            for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) 
                buffer[i+1] = board[i];
            
            
            send_to_all(buffer, sockfd_cli, nb_cli);
            send_to_all(buffer, sockfd_obs, nb_obs);
            
            send_board = FALSE;
        }
        

        //awaiting input from player
        if(nb_cli == MAX_PLAYER && socket_ready(sockfd_cli[current_player], 50))
        {
            memset(buffer, 0, BUFFER_SIZE);
            recv_verif(sockfd_cli[current_player], buffer);
            
            
            
            choice = buffer[0];
            printf("Player %c played color %c\n", player[current_player], choice);
            //rule checking
            if (choice < 'a' || choice >= 'a' + NB_COLORS) 
            {
                choice = rand() % NB_COLORS;
                choice += 'a';
                printf("Wrong input, player has been assigned color %c\n", choice);
            }
            
            //updating game state
            update_board(player[current_player] - 'a', choice - 'a', board);
            
            
            //changing player
            current_player = (current_player + 1) % nb_cli;
            
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
  

    send_to_all(buffer, sockfd_cli, 2);
  
    for (i = 0; i < nb_obs; i++)
        close(sockfd_obs[i]);
    free(sockfd_obs);
}
