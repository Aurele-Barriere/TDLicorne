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


int send_verif(int sockfd, char * msg)
{
    int sent = 0;
    int n = 0;
    int tries = 0;
    while(sent < BUFFER_SIZE && tries < MAX_TRIES)
    {
        tries += 1;
        n = send(sockfd, msg, BUFFER_SIZE, 0);
        sent += n;
    }
    if (tries >= MAX_TRIES)
    {
        printf("Data not sent\n");
        return -1;
    }
    return 0;
}

int recv_verif(int sockfd, char * buffer)
{
    int received = 0;
    int n = 0;
    int tries = 0;
    while(received < BUFFER_SIZE && tries < MAX_TRIES)
    {
        tries += 1;
        n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        received += n;
    }
    if (tries >= MAX_TRIES)
    {
        printf("Data not received\n");
        return -1;
    }
    return 0;
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


int wait_client(int sockfd)
{
    socklen_t clilen;
    struct sockaddr_in cli;

    int sockfd1 = accept(sockfd, (struct sockaddr *) &cli, &clilen);


    return sockfd1;
}






struct client_set client_set_init()
{
    struct client_set set = {NULL, 0};
    return set;
}

void client_set_add(struct client_set* set, int sockfd, char id)
{
    set->nb++;
    
    set->client = (struct client*) realloc(set->client, set->nb * sizeof(struct client));
    set->client[set->nb-1].sockfd = sockfd;
    set->client[set->nb-1].is_connected = TRUE;
    set->client[set->nb-1].id = id;
    set->client[set->nb-1].time = 0.0;
}

void client_set_send(struct client_set * set, char* msg)
{
    unsigned i;
    int er;
    for (i = 0; i < set->nb; i++)
    {
        er = send_verif(set->client[i].sockfd, msg);
        if (er == -1)
        {
            set->client[i].is_connected = 0;    //handling disconnecting players
            printf("error\n");
        }
    }
}

void client_set_close(struct client_set set)
{
    unsigned i;
    for (i = 0; i < set.nb; i++)
        close(set.client[i].sockfd);
    free(set.client);
}

