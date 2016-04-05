#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "network.h"
#include "render.h"
#include "board.h"
#include "strategy.h"


void game_7colors(char you, char (*strat)(char), int sockfd);

#endif