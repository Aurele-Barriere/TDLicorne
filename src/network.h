#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "defines.h"
/* 
   Networking functions
*/

// init a server by creating a new socket and binding it with then given port number
int init_server(const char* portno);

// init a client by creating a new socket and binding it with given port number and address
// 'type' will be sent to the server
int init_client(const char* portno, const char* addr, char type);

// Sends everything
void send_verif(int sockfd, char * msg);

// Receives everything
void recv_verif(int sockfd, char * buffer);

// Sends to n sockets
void send_to_all(char * msg, int* sockfd, unsigned n);

// Sends a single char to the given socket
void send_char(int sockfd, char msg);

// Non blocking. Returns 1 if sockfd is ready for reading, 0 otherwise. 
// timeout in ms
int socket_ready(int sockfd, unsigned timeout_ms);
