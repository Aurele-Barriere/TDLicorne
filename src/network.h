#include <sys/socket.h>
#include <arpa/inet.h>

/* 
   Networking functions
*/

// init a server by creating a new socket and binding it with then given port number
int init_server(const char* portno);

// init a client by creating a new socket and binding it with given port number and address
int init_client(const char* portno, const char* addr);

// Sends everything
void send_verif(int sockfd, char * msg);

// Receives everything
void recv_verif(int sockfd, char * buffer);

// Sends to both players (given the sockets)
void send_to_both(char * msg, int sockfd1, int sockfd2);

// Sends to n sockets
void send_to_all(char * msg, int* sockfd, unsigned n);

// Non blocking. Returns 1 if sockfd is ready for reading, 0 otherwise. 
// timeout in ms
int socket_ready(int sockfd, unsigned timeout_ms);
