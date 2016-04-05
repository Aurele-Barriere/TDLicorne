#include "render.h"



//prints board
void print_board(char * b)
{
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
            printf("%c ", get_cell(i, j, b));
        printf("\n");
    }
}


void display_board(SDL_Renderer* renderer, char* board)
{
    unsigned i, j;
    SDL_Rect rect;
    char color;
    bool tl, tr, tu, td; // test left, right, up, down
    int w, h;

    SDL_GetRendererOutputSize(renderer, &w, &h);

    unsigned block_width = w / BOARD_SIZE;
    unsigned block_height = h / BOARD_SIZE;


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );
    SDL_RenderClear(renderer);



    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            color = get_cell(i, j, board);

            rect.x = i * block_width;
            rect.y = j * block_height;
            rect.w = block_width;
            rect.h = block_height;

            if (block_width >= 4)
            {
                tl = (color == get_cell(i-1, j, board));
                tr = (color == get_cell(i+1, j, board));
                rect.x += !tl;
                rect.w -= !tr + !tl;
            }

            if (block_height >= 4)
            {
                tu = (color == get_cell(i, j-1, board));
                td = (color == get_cell(i, j+1, board));
                rect.y += !tu;
                rect.h -= !td + !tu;
            }

            switch(color)
            {
            case 'a':
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                break;
            case 'b':
                SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);
                break;
            case 'c':
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                break;
            case 'd':
                SDL_SetRenderDrawColor(renderer, 128, 255, 0, 255);
                break;
            case 'e':
                SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
                break;
            case 'f':
                SDL_SetRenderDrawColor(renderer, 95, 10, 190, 255);
                break;
            case 'g':
                SDL_SetRenderDrawColor(renderer, 238, 130, 238, 255);
                break;
            case '^':
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                break;
            case '@':
                SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            }

            SDL_RenderFillRect(renderer, &rect);

        }
    }


    SDL_RenderPresent(renderer);

}

void display_waiter(SDL_Renderer* renderer)
{
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    SDL_GetRendererOutputSize(renderer, &rect.w, &rect.h);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);

    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

