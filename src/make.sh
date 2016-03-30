#!/bin/bash

gcc server.c board.c network.c -o server
gcc client.c board.c network.c -o client
