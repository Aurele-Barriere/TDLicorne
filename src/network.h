
/** All functions related networking */
#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "defines.h"


// init a server by creating a new socket and binding it with then given port number
int init_server(const char* portno);

// init a client by creating a new socket and binding it with given port number and address
// 'type' will be sent to the server
int init_client(const char* portno, const char* addr, char type);

// Sends everything
int send_verif(int sockfd, char * msg);

// Receives everything
int recv_verif(int sockfd, char * buffer);


// Sends a single char to the given socket
void send_char(int sockfd, char msg);

// accepts a client connecting to the socket, and returns the new socket
int wait_client(int sockfd);


// Non blocking. Returns 1 if sockfd is ready for reading, 0 otherwise.
// timeout in ms
int socket_ready(int sockfd, unsigned timeout_ms);






struct client
{
    int sockfd;
    bool is_connected;
    char id; // the letter of the player
    double time;
};

// This structure handles a group of clients
struct client_set
{
    struct client* client;
    unsigned nb;
};

// Inits a client_set with defaults values
struct client_set client_set_init();

// Adds a socket to the client_set, with the given id (only for players)
void client_set_add(struct client_set* set, int sockfd, char id);

// Sends a buffer to all the sockets of the client_set
void client_set_send(struct client_set* set, char* msg);

// Close every sockets
void client_set_close(struct client_set set);

#endif
