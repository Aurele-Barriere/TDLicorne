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



#define BOARD_SIZE 10
#define NB_COLORS 7
// What symbol will be used for players. must be negative so there is no confusion with letters. 
// To see wich symbol, add 97 and check ascii table.
#define color1 -3 
#define color2 -33



#define BUFFER_SIZE 2000
#define MAX_TRIES 200

void error (char * msg);


#endif
