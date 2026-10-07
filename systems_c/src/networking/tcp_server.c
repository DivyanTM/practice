#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 4000

int main(void) {
  printf("RECEIVER CREATED.\n");

  int sockfd;
  struct sockaddr_in server_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("SOCKET CREATION FAILED.\n");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  listen(sockfd, 10);

  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

  printf("client conected\n");

  char buffer[2048];

  while (1) {
    ssize_t bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes < 0) {
      printf("Data receive failed!\n");
      break;
    }
    printf("-----------%s----------------\n", buffer);
  }

  return EXIT_SUCCESS;
}
