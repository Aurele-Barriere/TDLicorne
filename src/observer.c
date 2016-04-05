#include "observer.h"



int main(int argc, char * argv[])
{
    int sockfd;

    // Checking port and address as argument
    if (argc != 3)
        error("Usage : observer <portno> <host>");


    printf("Waiting...\n");
    sockfd = init_client(argv[1], argv[2], 'o');

    printf("Successfully connected to the server.\n\n");

    game_7colors(sockfd);

    return 0;
}


void game_7colors(int sockfd)
{
    bool keep_watching = TRUE;
    char buffer[BUFFER_SIZE];

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("7 colors : observer",SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;

    while(keep_watching)
    {
        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                keep_watching = FALSE;
        }


        if(socket_ready(sockfd, 50))
        {
            memset(buffer, 0 , BUFFER_SIZE);
            recv_verif(sockfd, buffer);
            display_board(renderer, buffer+1);
        }
    }

    close(sockfd);

    SDL_Delay(1000);

    SDL_DestroyWindow(window);
    SDL_Quit();
}
