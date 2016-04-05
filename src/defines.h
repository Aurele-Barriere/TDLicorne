/*
   Useful definitions
 */
#ifndef DEFINES_H
#define DEFINES_H

#include <stdlib.h>
#include <stdio.h>



typedef char bool;

#define TRUE 1
#define FALSE 0


#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 300


#define BOARD_SIZE 30
#define NB_COLORS 7

#define MAX_PLAYER 2
#define TIME_OUT 20.0

#define BUFFER_SIZE ((BOARD_SIZE)*(BOARD_SIZE)*2)
#define MAX_TRIES 200

void error(char* msg);


#endif
