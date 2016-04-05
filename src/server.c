#include "server.h"



int main(int argc, char * argv[])
{
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

double elapsed_time(struct timeval t1)
{
    struct timeval t2;
    gettimeofday (&t2, NULL);

    int u1 = (t1.tv_sec * 1000000 + t1.tv_usec);
    int u2 = (t2.tv_sec * 1000000 + t2.tv_usec);
    return (u2  - u1) / 1000000.;
}


void game_7colors(int sockfd)
{
    srand(time(NULL)); //initializing random
    // the 7 colors game
    printf("Starting game of 7 colors\n\n");
    int recv_error = 0;
    int winner = 0;
    char choice;
    char buffer [BUFFER_SIZE];
    double t;
    int i;

    int sock;


    struct timeval t_begin;


    struct client_set player = client_set_init();
    struct client_set obs = client_set_init();

    char symbols[] = {'^', '@'};
    int current_player = rand() % 2;

    bool send_board = FALSE;


    set_sym_board(); //initializing board
    set_cell(0, BOARD_SIZE-1, symbols[0], board);
    set_cell(BOARD_SIZE-1, 0, symbols[1], board);
    
    printf("The board has been generated.\n\n");

    printf("Waiting for players to connect.\n");
    printf("%d more players required !\n\n", MAX_PLAYER - player.nb);



    while(!(winner = game_over(board, player)))
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
                    client_set_add(&player, sock, symbols[player.nb]);

                    send_char(sock, player.client[player.nb-1].id);
                    printf("Player %c successfully connected !\n", player.client[player.nb-1].id);

                    if (player.nb == MAX_PLAYER)
                    {
                        printf("All the player are connected... Let's start the game !\n\n");
                        gettimeofday (&t_begin, NULL);
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
                client_set_add(&obs, sock, '\0');
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

            buffer[0] = player.client[current_player].id;
            for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++)
                buffer[i+1] = board[i];


            client_set_send(&player, buffer);
            client_set_send(&obs, buffer);

            send_board = FALSE;
        }


        //awaiting input from player
        if(player.nb == MAX_PLAYER)
        {
            t = elapsed_time(t_begin);
            printf("       \rtime left : %f", TIME_OUT - t);
            fflush(stdout);

            if (socket_ready(player.client[current_player].sockfd, 50) || t >= TIME_OUT)
            {
                // If time is out
                if (t >= TIME_OUT)
                {
                    choice = rand() % NB_COLORS + 'a';
                    printf("\nTime out, player has been assigned color %c\n", choice);
                }
                else
                {
                    memset(buffer, 0, BUFFER_SIZE);
                    recv_error = recv_verif(player.client[current_player].sockfd, buffer);
                    if (recv_error == -1)
                    {
                        player.client[current_player].is_connected = 0;
                    }
                    choice = buffer[0];
                }
                
                player.client[current_player].time += t;

                printf("\nPlayer %c played color %c\n\n", player.client[current_player].id, choice);
                //rule checking
                if (choice < 'a' || choice >= 'a' + NB_COLORS)
                {
                    choice = rand() % NB_COLORS + 'a';
                    printf("Wrong input, player has been assigned color %c\n", choice);
                }

                //updating game state
                update_board(player.client[current_player].id, choice, board);


                //changing player
                current_player = (current_player + 1) % player.nb;

                send_board = TRUE;
                gettimeofday (&t_begin, NULL);
            }
        }
    }


    // end of game

    memset(buffer, 0, BUFFER_SIZE);
    buffer[0] = '*';
    buffer[1] = winner;

    printf("Game over !\n\n");
    
    printf("Time for player %c : %f\n", player.client[0].id, player.client[0].time);
    printf("Time for player %c : %f\n\n", player.client[1].id, player.client[1].time);
    
    if (winner == -1)
        printf("Draw\n");
    else
        printf("Player %c won !\n", winner);
    

    client_set_send(&player, buffer);
    client_set_send(&player, buffer); // we must send it twice if one of the player expect the other player's move



    client_set_close(player);
    client_set_close(obs);
}
