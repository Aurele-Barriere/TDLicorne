#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

#include "board.h"
#include "defines.h"


/** Printing a given board */
void print_board(char *b);

/** Displays the board on a SDL window */
void display_board(SDL_Renderer* renderer, char* board);

/** Displays a transparent black square, meaning its not your turn */
void display_waiter(SDL_Renderer* renderer);

#endif