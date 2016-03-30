#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "defines.h"

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
char other(char color) {
  if (color == color1) {
    return color2;
  }
  else {
    return color1;
  }
}

//copying board into test_board
void copy_board() {
  int i;
  int j;
  for (i = 0; i<BOARD_SIZE; i++) {
    for (j=0; j<BOARD_SIZE; j++) {
      set_cell(i,j,get_cell(i,j,board),test_board);
    }
  }
}

//prints board
void print_board(char * b)
{
   int i, j;
   printf("Score1: %d, Score2: %d\n", score(b,color1), score(b,color2));
   printf("Frontier1: %d, Frontier2: %d\n", frontier(b,color1), frontier(b,color2));
   for (i=0; i<BOARD_SIZE; i++) {
      for (j=0; j<BOARD_SIZE; j++)
	printf("%c ", get_cell(i, j, b)+97);
      printf("\n");
   }
}

//checking board range
int in_board(int x, int y) {
  if (x>=0 && x< BOARD_SIZE && y>=0 && y<BOARD_SIZE) {
    return 1;}
  else {return 0;}
}

//update, given player, choice and board
void update_board(char player, char color, char * b) {
  int i,j;
  int change = 0;
  for (i=0; i<BOARD_SIZE; i++) {
    for (j=0; j<BOARD_SIZE; j++) {
      if (get_cell(i,j,b) == color) {
	if (in_board(i-1,j)) {if (get_cell(i-1,j,b) == player) {set_cell(i,j,player,b); change = 1;}}
	if (in_board(i+1,j)) {if (get_cell(i+1,j,b) == player) {set_cell(i,j,player,b); change = 1;}}
	if (in_board(i,j-1)) {if (get_cell(i,j-1,b) == player) {set_cell(i,j,player,b); change = 1;}}
	if (in_board(i,j+1)) {if (get_cell(i,j+1,b) == player) {set_cell(i,j,player,b); change = 1;}}
      }
    }
  }
  if (change) {update_board(player, color, b );}
}

//creates random board
void set_random_board() {
  int i, j;
  int r = 0;
  //RAND_MAX = NB_COLORS;
  for (i = 0; i< BOARD_SIZE; i++){
    for (j=0; j < BOARD_SIZE; j++) {
      r = rand() % NB_COLORS;
      set_cell(i,j,r,board);
    }
  }
  set_cell(0,BOARD_SIZE-1, color1,board);
  set_cell(BOARD_SIZE-1,0, color2,board);
}

//creates symetric board
void set_sym_board() {
  int i, j;
  int r = 0;
  //first half
  for (i = 0; i< BOARD_SIZE; i++){
    for (j=i; j < BOARD_SIZE; j++) {
      r = rand() % NB_COLORS;
      set_cell(i,j,r,board);
    }
  }
  //second half
  for (i = 0; i<BOARD_SIZE; i++) {
    for (j = 0; j<i; j++) {
      set_cell(i,j,get_cell(j,i,board),board);
    }
  }
  set_cell(0,BOARD_SIZE-1, color1,board);
  set_cell(BOARD_SIZE-1,0, color2,board);
}

//calculates the score of a player on a board
int score (char * b, char color) {
  int i;
  int j;
  int s = 0;
  for (i = 0; i<BOARD_SIZE; i++) {
    for (j = 0; j<BOARD_SIZE; j++) {
      if (get_cell(i,j,b) == color) {
	s += 1;
      }
    }
  }
  return s;
}

//frontier for hegemony
int frontier (char * b, char color) {
  int f = 0;
  int in_frontier = 0;
  int i;
  int j;
  for (i = 0; i<BOARD_SIZE; i++) {
    for (j = 0; j<BOARD_SIZE; j++) {
      in_frontier = 0;
      if (get_cell(i,j,b) != color) {
	if (in_board(i,j+1)) {
	  if (get_cell(i,j+1,b)==color) {
	    in_frontier = 1;
	  }}
	if (in_board(i,j-1)) {
	  if (get_cell(i,j-1,b)==color) {
	    in_frontier = 1;
	    }}
	if (in_board(i+1,j)) {
	  if (get_cell(i+1,j,b)==color) {
	    in_frontier = 1;
	    }}
	if (in_board(i-1,j)) {
	  if (get_cell(i-1,j,b)==color) {
	    in_frontier = 1;
	    }}
	f += in_frontier;
      }
    }
  }
  return f;
}

//available space for starve strategy
int available(char* b, char color) {
  int matrix[BOARD_SIZE * BOARD_SIZE] = {0};
  int av = 0;
  int otherPlayer = other(color);
  char change = 1;
  int i = 0;
  int j = 0;
  for (i = 0; i<BOARD_SIZE; i++) {
    for (j = 0; j<BOARD_SIZE; j++) {
      if (get_cell(i,j,b) == color) {
        matrix[j*BOARD_SIZE + i] = 1;
        av += 1;
      }
    }
  }
  while (change) {
    change = 0;
    for (i = 0; i < BOARD_SIZE; i++) {
      for (j = 0; j < BOARD_SIZE; j++) {
        if (get_cell(i,j,b) != otherPlayer && !matrix[j*BOARD_SIZE + i]){
          if (in_board(i+1, j)) {
            if (matrix[j*BOARD_SIZE + i+1]){
              matrix[j*BOARD_SIZE + i] = 1;
              change = 1;
            }
          }
          if (in_board(i-1, j)) {
            if (matrix[j*BOARD_SIZE + i-1]){
              matrix[j*BOARD_SIZE + i] = 1;
              change = 1;
            }
          }
          if (in_board(i, j+1)) {
              if (matrix[(j+1)*BOARD_SIZE + i]) {
                matrix[j*BOARD_SIZE + i] = 1;
                change = 1;
              }
          }
          if (in_board(i, j-1)) {
            if (matrix[(j-1)*BOARD_SIZE + i]) {
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
int personal_space(char * b, char color) {
  return ((BOARD_SIZE*BOARD_SIZE) - available(b, other(color)));
}

//mixing score and frontier
int mix(char * b, char color) {
  return score(b,color) + frontier(b,color);
}
