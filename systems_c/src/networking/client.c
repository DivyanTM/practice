#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 4000
#define BUFFER_SIZE 1024

int main(void) {

  int sockFd;
  struct sockaddr_in server_addr;

  char *msg = "HELLO THIS IS DIVYAN";
  printf("CLIENT PROGRAM SENDING DATA.........\n");

  sockFd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockFd < 0) {
    perror("SOCKET CREATION FAILED\n");
    exit(EXIT_FAILURE);
  }

  printf("SOCKET CREATED\n");
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("Not Valid Server Address.\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    ssize_t bytes =
        sendto(sockFd, msg, strlen(msg), 0, (struct sockaddr *)&server_addr,
               sizeof(server_addr));

    if (bytes < 0) {
      perror("SEND FAILED\n");
      exit(EXIT_FAILURE);
    }
  }
  close(sockFd);

  return EXIT_SUCCESS;
}
