#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 4000

int main(void) {
  printf("SENDER CREATED.\n");

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
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Connect failed");
    close(sockfd);
    return 1;
  }

  char *msg = "HI THIS IS DIVYAN!";
  while (1) {
    write(sockfd, msg, strlen(msg));
  }

  close(sockfd);

  return EXIT_SUCCESS;
}
