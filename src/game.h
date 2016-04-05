/** All functions related to the rules of the game of 7 colors */
#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "network.h"
#include "defines.h"

/** Given a player and its choice, update the given board */
void update_board(char player, char color, char * b);

/** Returns 0 if the game is not over, -1 if it is a draw, of the winner playe otherwise */
int game_over(char* board, struct client_set player);



#endif
