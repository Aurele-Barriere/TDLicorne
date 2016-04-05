#include "board.h"

int printing = 1;

char      board[BOARD_SIZE * BOARD_SIZE] = { 0 };
char test_board[BOARD_SIZE * BOARD_SIZE] = { 0 };

//get function. b points to a board
char get_cell(int x, int y, char * b)
{
    if (x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE)
        return -1;
    return b[y*BOARD_SIZE + x];
}

//set function. b points to a board
void set_cell(int x, int y, char color, char * b)
{
    b[y*BOARD_SIZE + x] = color;
}

//returns color of the other player
char other(char color)
{
    char c;


    int i;
    int j;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            c = get_cell(i, j, board);
            if (c != color && (c < 'a' || c >= 'a' + NB_COLORS))
                return c;
        }
    }

    return '\0';
}

//copying board into test_board
void copy_board()
{
    int i;
    int j;
    for (i = 0; i<BOARD_SIZE; i++)
    {
        for (j=0; j<BOARD_SIZE; j++)
        {
            set_cell(i,j,get_cell(i,j,board),test_board);
        }
    }
}


//checking board range
int in_board(int x, int y)
{
    if (x>=0 && x< BOARD_SIZE && y>=0 && y<BOARD_SIZE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



//creates random board
void set_random_board()
{
    int i, j;
    int r = 0;
    //RAND_MAX = NB_COLORS;
    for (i = 0; i< BOARD_SIZE; i++)
    {
        for (j=0; j < BOARD_SIZE; j++)
        {
            r = rand() % NB_COLORS + 'a';
            set_cell(i,j,r,board);
        }
    }
}

//creates symetric board
void set_sym_board()
{
    int i, j;
    int r = 0;
    //first half
    for (i = 0; i< BOARD_SIZE; i++)
    {
        for (j=i; j < BOARD_SIZE; j++)
        {
            r = rand() % NB_COLORS + 'a';
            set_cell(i,j,r,board);
        }
    }
    //second half
    for (i = 0; i<BOARD_SIZE; i++)
    {
        for (j = 0; j<i; j++)
        {
            set_cell(i,j,get_cell(j,i,board),board);
        }
    }
}

//calculates the score of a player on a board
int score (char * b, char color)
{
    int i;
    int j;
    int s = 0;
    for (i = 0; i<BOARD_SIZE; i++)
    {
        for (j = 0; j<BOARD_SIZE; j++)
        {
            if (get_cell(i,j,b) == color)
            {
                s += 1;
            }
        }
    }
    return s;
}

//frontier for hegemony
int frontier (char * b, char color)
{
    int f = 0;
    int i;
    int j;
    for (i = 0; i<BOARD_SIZE; i++)
    {
        for (j = 0; j<BOARD_SIZE; j++)
        {
            if (get_cell(i,j,b) != color)
            {
                if (get_cell(i,j+1,b) == color ||
                        get_cell(i,j-1,b) == color ||
                        get_cell(i+1,j,b) == color ||
                        get_cell(i-1,j,b) == color)
                    f++;
            }
        }
    }
    return f;
}

//available space for starve strategy
int available(char* b, char color)
{
    int matrix[BOARD_SIZE * BOARD_SIZE] = {0};
    int av = 0;
    int otherPlayer = other(color);
    char change = 1;
    int i = 0;
    int j = 0;
    for (i = 0; i<BOARD_SIZE; i++)
    {
        for (j = 0; j<BOARD_SIZE; j++)
        {
            if (get_cell(i,j,b) == color)
            {
                matrix[j*BOARD_SIZE + i] = 1;
                av += 1;
            }
        }
    }
    while (change)
    {
        change = 0;
        for (i = 0; i < BOARD_SIZE; i++)
        {
            for (j = 0; j < BOARD_SIZE; j++)
            {
                if (get_cell(i,j,b) != otherPlayer && !matrix[j*BOARD_SIZE + i])
                {
                    if (in_board(i+1, j))
                    {
                        if (matrix[j*BOARD_SIZE + i+1])
                        {
                            matrix[j*BOARD_SIZE + i] = 1;
                            change = 1;
                        }
                    }
                    if (in_board(i-1, j))
                    {
                        if (matrix[j*BOARD_SIZE + i-1])
                        {
                            matrix[j*BOARD_SIZE + i] = 1;
                            change = 1;
                        }
                    }
                    if (in_board(i, j+1))
                    {
                        if (matrix[(j+1)*BOARD_SIZE + i])
                        {
                            matrix[j*BOARD_SIZE + i] = 1;
                            change = 1;
                        }
                    }
                    if (in_board(i, j-1))
                    {
                        if (matrix[(j-1)*BOARD_SIZE + i])
                        {
                            matrix[j*BOARD_SIZE + i] = 1;
                            change = 1;
                        }
                    }
                    av += matrix[j*BOARD_SIZE + i];
                }
            }
        }
    }
    return av;
}

//space that is guaranteed for you
int personal_space(char * b, char color)
{
    return ((BOARD_SIZE*BOARD_SIZE) - available(b, other(color)));
}

//mixing score and frontier
int mix(char * b, char color)
{
    return score(b,color) + frontier(b,color);
}
