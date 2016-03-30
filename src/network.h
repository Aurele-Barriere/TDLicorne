#include <sys/socket.h>

/* 
   Networking functions
*/


// Sends everything
void send_verif(int sockfd, char * msg);

// Receives everything
void recv_verif(int sockfd, char * buffer);

// Sends to both players (given the sockets)
void send_to_both(char * msg, int sockfd1, int sockfd2);
