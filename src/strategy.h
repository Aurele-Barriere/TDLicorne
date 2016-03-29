/** Strategies that can be used in the game */
#ifndef STRATEGY_H
#define STRATEGY_H

/** Chooses at random between all colors */
char alea(char player);

/** Chosses at random between colors that are useful at this turn */
char alea_useful_colors(char player);

/** Bad attempt at greedy strategy */
char wrong_greedy(char player);

/** Good attempt at greedy strategy */
char greedy(char player);

/** Ask the player his choice */
char player_choice(char player);

/** Maximize the frontier */
char hegemony(char player);

/** Minimize available space for the other player */
char starve(char player);

/** Given a function to maximize, chooses the best letter */
char maximize(int (*f) (char *, char), char player);

/** Maximizes score + frontier */
char greedymony(char player);

#endif
