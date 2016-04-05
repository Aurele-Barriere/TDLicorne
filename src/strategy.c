
#include "strategy.h"

//random
char alea (char player)
{
    char r =  rand() % 7;
    if(printing)
    {
        printf("Color chose by random AI : %c for player %c\n", r+97,player);
    }
    return r;
}

//random between useful colors
char alea_useful_colors(char player)
{
    //chooses at random between colors that can make any progress
    int useful[NB_COLORS] = {0};
    int i,j;
    char color;
    int c;

    for (i = 0; i< BOARD_SIZE; i++)
    {
        for (j = 0; j< BOARD_SIZE; j++)
        {
            color = get_cell(i,j,board);
            if ('a' <= color && color < 'a' + NB_COLORS)
            {
                c = (int) (color - 'a');
                if (useful[c] == 0)
                {
                    if (get_cell(i-1,j,board) == player ||
                        get_cell(i+1,j,board) == player ||
                        get_cell(i,j-1,board) == player ||
                        get_cell(i,j+1,board) == player)
                        useful[c] = 1;
                }
            }
        }
    }
    int sum = 0;
    for (i = 0; i< NB_COLORS; i++)
    {
        if (useful[i])
        {
            sum++;
        }
    }

    int r = 0;
    if (sum>0)
    {
        r =  rand() % sum;
    }

    char decision = (char) 0;
    int choice = -1;
    for (i=0; i< NB_COLORS; i++)
    {
        if (useful[i])
        {
            choice++;
        }
        if (choice == r)
        {
            decision = (char) i;
            break;
        }
    }
    if(printing)
    {
        printf("\nColor chose by random_useful AI : %c\n", i+'a');
    }
    return decision + 'a';
}

//wrong greedy
char wrong_greedy(char player)
{
    int occurrences[NB_COLORS] = {0};
    int i,j;
    char color;
    int c;

    for (i = 0; i< BOARD_SIZE; i++)
    {
        for (j = 0; j< BOARD_SIZE; j++)
        {
            color = get_cell(i,j,board);
            if ('a' <= color && color < 'a' + NB_COLORS)
            {
                c = (int) (color - 'a');

                if (get_cell(i-1,j,board) == player || 
                    get_cell(i+1,j,board) == player || 
                    get_cell(i,j-1,board) == player || 
                    get_cell(i,j+1,board) == player)
                    occurrences[c]++;
            }
        }
    }
    int max = 0;
    char letter = 0;
    for (i = 0; i<NB_COLORS; i++)
    {
        if (occurrences[i] > max)
        {
            max = occurrences[i];
            letter = i + 'a';
        }
    }
    if(printing)
    {
        printf("\nColor chose by wrong greedy AI : %c\n", letter);
    }
    return letter;
}

//ask for the player's choice
char player_choice(char player)
{
    char c;
    printf("What color do you choose, player %c ?\n", player);
    printf("Must be between a-g  \n");
    c = getchar();
    getchar();
    if ('a' <= c && c < 'a' + NB_COLORS)
        return c;
    return player_choice(player);
}



//general strategy : takes a function f to maximize
char maximize(int (*f) (char *, char), char player)
{
    int i =0;
    int j =0;
    char color;
    int c;
    int val[NB_COLORS] = { 0 };
    for (i = 0; i < NB_COLORS; i++)
    {
        copy_board();
        update_board(player, i+'a', test_board);
        val[i] = f(test_board, player);
    }


    //choosing only from useful colors
    int useful[NB_COLORS] = { 0 };

    for (i = 0; i< BOARD_SIZE; i++)
    {
        for (j = 0; j< BOARD_SIZE; j++)
        {
            color = get_cell(i,j,board);
            if ('a' <= color && color < 'a' + NB_COLORS)
            {
                c = (int) (color - 'a');
                if (useful[c] == 0)
                {
                    if (get_cell(i-1,j,board) == player ||
                        get_cell(i+1,j,board) == player ||
                        get_cell(i,j-1,board) == player ||
                        get_cell(i,j+1,board) == player)
                        useful[c] = 1;
                    
                }
            }
        }
    }

    //removing useless colors

    for (i=0; i<NB_COLORS; i++)
    {
        val[i] *= useful[i];
    }

    //now getting the max out of it
    int max = 0;
    char choice = 0;
    for (i=0; i<NB_COLORS; i++)
    {
        if (val[i]>=max)
        {
            max = val[i];
            choice = i + 'a';
        }
    }
    if(printing)
    {
        printf("color chosen by AI : %c\n", choice);
    }
    return choice;
}

// maximizes score
char greedy(char player)
{
    return maximize(score, player);
}

// maximizes frontier
char hegemony(char player)
{
    return maximize(frontier, player);
}

// maximizes guaranteed space
char starve(char player)
{
    return maximize(personal_space, player);
}

//maximizes score + frontier
char greedymony(char player)
{
    return maximize(mix, player);
}
