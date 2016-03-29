/*
  Aurele Barriere & Nathan Thomasset
  ARCSYS 2
*/

extern int printing;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defines.h"
#include "board.h"
#include "strategy.h"


/** Victory condition */
int victory(int score1, int score2) {
  int limit = (BOARD_SIZE*BOARD_SIZE)/2;
  if (score1>limit || score2>limit) {
    return 1;
  }
  return 0;
}


/** Draw condition */
int draw(int score1, int score2) {
  int limit = (BOARD_SIZE*BOARD_SIZE)/2;
  if (score1 == limit && score2 == limit) {
    return 1;
  }
  return 0;
}


/** Game. returns the number of the winner (1 or 2 or 0 for draw). Takes 2 strategies as arguments */
int game(char (*strat1)(char), char (*strat2)(char)) {
  int player =  rand() % 2; //which player begins
  int score1 = 0;
  int score2 = 0;
  char choice ;

  if(printing){printf("\n\n  Welcome to the 7 wonders of the wonderful world of the 7 wonderful colors\n");}
   set_sym_board();

   while(!victory(score1, score2) && !draw(score1,score2)) {
     if(printing){print_board(board);}
     if (player) {choice =strat2(color2);}
     else {choice = strat1(color1);}
     if (player) {update_board(color2, choice, board);}
     else {update_board(color1, choice, board);}
     player = 1-player; //changing player
     score1 = score(board,color1);
     score2 = score(board,color2);
     if(printing){printf("Score 1 : %d%%\tScore 2 : %d%%\n",score1*100/(BOARD_SIZE*BOARD_SIZE),score2*100/(BOARD_SIZE*BOARD_SIZE));}
   }

   if (draw(score1, score2)) {return 0;}
   if (score1>score2) {return 1;}
   if (score2>score1) {return 2;}

  return 0;
}


/** Program entry point */
int main()
{
  printing = 0; //it will change if the user plays 
  srand(time(NULL)); //initializing random
  int i;
  int choice1 = 0;
  int choice2 = 0;
  int nb_games = 1;
  char (*strat1)(char);
  char (*strat2)(char);
  int win1 = 0;
  int win2 = 0;
  int draws = 0;
  int result;

  //introducing the game
  printf("Welcome to the game of seven colors\n");
  printf("How many games do you want to play?\n");
  scanf("%d", &nb_games);
  printf("\n%d games will be played\nChoose first strategy :\n1:alea\n2:alea_useful_colors\n3:greedy\n4:hegemony\n5:starve\n6:player_choice\n7:greedymony (mixing greedy and hegemony)\n", nb_games);
  scanf("%d", &choice1);
  printf("\nChoose second strategy :\n1:alea\n2:alea_useful_colors\n3:greedy\n4:hegemony\n5:starve\n6:player_choice\n7:greedymony (mixing greedy and hegemony)\n");
  scanf("%d", &choice2);
  if (choice1<1 || choice1>7 || choice2<1 || choice2>7) {
    printf("Invalid choices. No games will be played\n");
    return 0;
  }

  switch (choice1) {
  case 1: strat1 = alea; break;
  case 2: strat1 = alea_useful_colors; break;
  case 3: strat1 = greedy; break;
  case 4: strat1 = hegemony; break;
  case 5: strat1 = starve; break;
  case 6: strat1 = player_choice; printing = 1; break;
  default: strat1 = greedymony;
  }

  switch (choice2) {
  case 1: strat2 = alea; break;
  case 2: strat2 = alea_useful_colors; break;
  case 3: strat2 = greedy; break;
  case 4: strat2 = hegemony; break;
  case 5: strat2 = starve; break;
  case 6: strat2 = player_choice; printing = 1; break;
  default: strat2 = greedymony;
  }
  
  getchar();//cleaning buffer for player's choice

  // launching the games
  for (i = 0; i<nb_games; i++) {
    result = game(strat1, strat2);
    printf("%d", result);
    fflush(stdout); //printing results as they come
    switch(result) {
    case 1: win1+=1; break;
    case 2: win2+=1; break;
    default: draws+=1; break;
    }
  }

  //final results 
  printf("\nPlayer 1 won %d games\n", win1);
  printf("Player 2 won %d games\n", win2);
  printf("There has been %d draws\n", draws);

  return 0;
}
