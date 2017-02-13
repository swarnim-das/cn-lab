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
  int srv_sock;
  if((srv_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket()");
    return EXIT_FAILURE;
  }
  
  int y = 1;
  setsockopt(srv_sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y));
  
  memset(&srv, 0, sizeof(srv));
  srv.sin_family = AF_INET; 
  srv.sin_port = htons(10000);
  srv.sin_addr.s_addr = INADDR_ANY;
  
  if(bind(srv_sock, (struct sockaddr *) &srv, sizeof(srv)) == -1) {
    perror("bind()");
    return EXIT_FAILURE;
  }
  
  char buf[2000];
  socklen_t cli_len;
  size_t len = recvfrom(srv_sock, buf, sizeof(buf), 0, (struct sockaddr *)&cli, &cli_len);
  printf("len = %d\n", len);
  if(len == -1) {
    perror("recvfrom()");
    return EXIT_FAILURE;
  }
  buf[len] = '\0';
  
  printf("filename: %s\n, from: %s %d", buf, inet_ntoa(cli.sin_addr), cli.sin_port);
  
  FILE *fp = fopen(buf, "r");
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  fread(buf, size, 1, fp);
  buf[size] = '\0';
  
  if(sendto(srv_sock, buf, strlen(buf), 0, (struct sockaddr *)&cli, sizeof(cli)) == -1) {
    perror("sendto()");
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

