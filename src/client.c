#include "client.h"


void game_7colors(char you, int sockfd)
{
    printf("Beginning of game of 7 colors\n");

    printf("Your color is %c\n", you);

    char choice;
    char buffer [BUFFER_SIZE];
    int player = 0;
    int winner = 0;
    int i;

    int choice_strat;
    char (*strat)(char) = NULL;




    printf("\nChoose a strategy :\n");
    printf("  1. alea\n");
    printf("  2. alea_useful_colors\n");
    printf("  3. greedy\n");
    printf("  4. hegemony\n");
    printf("  5. starve\n");
    printf("  6. greedymony (mixing greedy and hegemony)\n");
    printf("  7. player choice\n");
    if(scanf("%d", &choice_strat) != 1)
        choice_strat = 0;

    switch (choice_strat)
    {
    case 1:
        strat = alea;
        break;
    case 2:
        strat = alea_useful_colors;
        break;
    case 3:
        strat = greedy;
        break;
    case 4:
        strat = hegemony;
        break;
    case 5:
        strat = starve;
        break;
    case 6:
        strat = greedymony;
        break;
    case 7:
        strat = NULL;
        break;
    default:
        strat = NULL;
        printf("Invalid choice. You are a human player.\n\n");
        break;
    }


    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("7 colors : player", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;



    memset(buffer, 0, BUFFER_SIZE);

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
                if (player == you && strat == NULL) //asking for input if needed
                {
                    choice = get_cell(event.motion.x / (WINDOW_WIDTH / BOARD_SIZE),
                                      event.motion.y / (WINDOW_HEIGHT / BOARD_SIZE),
                                      board) + 'a';
                    memset(buffer, 0, BUFFER_SIZE);
                    buffer[0] = choice;
                    send_verif(sockfd, buffer);
                    player = 0;
                }
            default:
                break;
            }
        }


        // if non-human player
        if (player == you && strat != NULL)
        {
            choice = strat(you - 'a') + 'a';
            memset(buffer, 0, BUFFER_SIZE);
            buffer[0] = choice;
            send_verif(sockfd, buffer);
            player = 0;
        }


        //receiving game state and player
        if(socket_ready(sockfd, 50)) // non-blocking
        {
            memset(buffer, 0 , BUFFER_SIZE);
            recv_verif(sockfd, buffer);

            player = buffer[0];

            for(i = 0; i < BOARD_SIZE*BOARD_SIZE; i++)
                board[i] = buffer[i+1];


            //checkwinnering for end condition :
            // if buffer[0] == '*', the game is over, and buffer[1] contains the winner
            if (buffer[0] == '*')
                winner = buffer[1];
            else
            {
                display_board(renderer, board);
                
                if (player == you) //asking for input if needed
                    printf("It is your turn !\n");
                else
                {
                    printf("Your opponent is playing, please wait...\n");
                    display_waiter(renderer);
                }
            }
        }
    }


    // end of game
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



int main(int argc, char * argv[])
{
    int sockfd;
    char player;

    char buffer[BUFFER_SIZE];

    // Checking port and address as argument
    if (argc != 3)
    {
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
