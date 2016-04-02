#include "network.h"



int init_server(const char* portno)
{
    struct sockaddr_in serv_addr;
    int sockfd;
 

    // Creating socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) 
        error("creating socket");

    // Configure serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(portno));

    // binding socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0) 
        error("binding");
    
    
    return sockfd;
}

int init_client(const char* portno, const char* addr, char type)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    
    if (sockfd < 0) 
        error("creating socket");

    // Configure serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(atoi(portno));

    //connecting
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error(" connecting ");
    
    send_char(sockfd, type);
    
    return sockfd;
}


void send_verif(int sockfd, char * msg) {
  int sent = 0;
  int n = 0;
  int tries = 0;
  while(sent < BUFFER_SIZE && tries < MAX_TRIES) {
    tries += 1;
    n = send(sockfd, msg, BUFFER_SIZE, 0);
    sent += n;
  }
  if (tries >= MAX_TRIES) {
    error("Data not sent");
  }
}

void recv_verif(int sockfd, char * buffer) {
  int received = 0;
  int n = 0;
  int tries = 0;
  while(received < BUFFER_SIZE && tries < MAX_TRIES) {
    tries += 1;
    n = recv(sockfd, buffer, BUFFER_SIZE, 0);
    received += n;
  }
  if (tries >= MAX_TRIES) {
    error("Data not received");
  }
}

void send_to_all(char * msg, int* sockfd, unsigned n)
{
    unsigned i;
    for (i = 0; i < n; i++)
        send_verif(sockfd[i], msg);
}

void send_char(int sockfd, char msg)
{
    char buffer[BUFFER_SIZE];
    
    memset(buffer, 0 , BUFFER_SIZE);
    buffer[0] = msg;
    
    send_verif(sockfd, buffer);
}


int socket_ready(int sockfd, unsigned timeout_ms)
{
    fd_set readfs;
    struct timeval timeout;
    
    timeout.tv_sec = 0; // 0s
    timeout.tv_usec = timeout_ms * 1000; // to µs
    
    FD_ZERO(&readfs);
    FD_SET(sockfd, &readfs);  
    
    if (select(sockfd + 1, &readfs, NULL, NULL, &timeout) > 0)
        if(FD_ISSET(sockfd, &readfs))
            return 1;
    return 0;
}

void error (char * msg) {
  printf("\n error : %s\n", msg);
  exit(1);
}
