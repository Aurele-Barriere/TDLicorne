#ifndef OBSERVER_H
#define OBSERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "network.h"
#include "render.h"


void game_7colors(int sockfd);

#endif