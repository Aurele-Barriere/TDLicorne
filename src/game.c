#include "game.h"


//update, given player, choice and board
void update_board(char player, char color, char * b)
{
    int i,j;
    int change = 0;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (get_cell(i,j,b) == color)
            {
                if (get_cell(i-1,j,b) == player ||
                        get_cell(i+1,j,b) == player ||
                        get_cell(i,j-1,b) == player ||
                        get_cell(i,j+1,b) == player)
                {
                    set_cell(i,j,player,b);
                    change = 1;
                }
            }
        }
    }
    if (change)
        update_board(player, color, b);
}




int game_over(char* board, struct client_set player)
{
    if (player.nb != 2)
        return 0;
    
    int score1 = score(board, player.id[0]);
    int score2 = score(board, player.id[1]);
    int limit = (BOARD_SIZE*BOARD_SIZE)/2;

    if (score1 > limit)
        return  player.id[0];
    if (score2 > limit)
        return  player.id[1];
    if (score1 == limit && score2 == limit)
        return -1;
    
    
    //when a player disconnects, his opponent wins
    if (!player.is_connected[0])
    {
        printf("First player disconnected !\n");
        return player.id[1];
    }
    if (!player.is_connected[1])
    {
        printf("Second player disconnected !\n");
        return player.id[0];
    }
    
    return 0;
}

