#include "observer.h" 



int main(int argc, char * argv[]) {
  int sockfd, portno;
  struct sockaddr_in serv_addr;
  int i;
  
  char buffer[BUFFER_SIZE];

  // Checking port and address as argument
  if (argc != 3) {
    error("Usage : observer <portno> <host>");
  }

  portno = atoi(argv[1]);

  // Creating socket
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {error("creating socket");}

 // Configure serv_addr
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
  serv_addr.sin_port = htons(portno);

  //connecting
  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error(" connecting ");
  }

  // sending / recieving  
  memset(buffer, 0, BUFFER_SIZE);
  recv_verif(sockfd, buffer);
  if (buffer[0] != 'o')
      error(" you are not an observer");
  
  
  while(1)
  {
    memset(buffer, 0 , BUFFER_SIZE);
    recv_verif(sockfd, buffer);
    
    for (i = 0; i < BOARD_SIZE*BOARD_SIZE; i++)
        board[i] = buffer[i+1];
    print_board(board);
  }

  close(sockfd);
  return 0;
}
