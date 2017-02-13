#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <error.h>
#include <string.h>
#include <memory.h>

int main(int argc, char *argv[]) {
  struct sockaddr_in srv, cli;
  int cli_sock;
  if((cli_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket()");
    return EXIT_FAILURE;
  }
  
  int y = 1;
  setsockopt(cli_sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y));
  
  memset(&srv, 0, sizeof(srv));
  srv.sin_family = AF_INET; 
  srv.sin_port = htons(10000);
  srv.sin_addr.s_addr = INADDR_ANY;
  
  
  memset(&cli, 0, sizeof(cli));
  cli.sin_family = AF_INET; 
  cli.sin_port = htons(10001);
  cli.sin_addr.s_addr = INADDR_ANY;
  
  if(bind(cli_sock, (struct sockaddr *) &cli, sizeof(cli)) == -1) {
    perror("bind()");
    return EXIT_FAILURE;
  }
  
  char buf[2000];
  
  printf("Enter a filename: ");
  scanf("%s", buf);
  
  if(sendto(cli_sock, buf, strlen(buf), 0, (struct sockaddr *)&srv, sizeof(srv)) == -1) {
    perror("sendto()");
    return EXIT_FAILURE;
  }
  
  socklen_t cli_len;
  size_t len = recvfrom(cli_sock, buf, sizeof(buf), 0, (struct sockaddr *)&srv, &cli_len);
  printf("len = %d\n", len);
  if(len == -1) {
    perror("recvfrom()");
    return EXIT_FAILURE;
  }
  buf[len] = '\0';
  printf("%s\n", buf);
  
  return EXIT_SUCCESS;
}

