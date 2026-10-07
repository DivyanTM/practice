#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 4000

int main(void) {

  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[2048];
  socklen_t client_addr_length = sizeof(client_addr);

  printf("STARTING RECEIVER........\n");
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sockfd < 0) {
    perror("SOCKET CREATION FAILED\n");
    exit(EXIT_FAILURE);
  }

  printf("SOCKET CREATED\n");

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("SOCKET BINDING FAILED\n");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  while (1) {
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes =
        recvfrom(sockfd, &buffer, sizeof(buffer) - 1, 0,
                 (struct sockaddr *)&client_addr, &client_addr_length);

    if (bytes < 0) {
      perror("Receive Failed\n");
      break;
    }

    buffer[bytes] = '\0';
    printf("received : %s\n", buffer);
  }

  close(sockfd);
  return 0;
}
