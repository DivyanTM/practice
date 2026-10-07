#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 4000
#define BUFFER_SIZE 2048

int main(int argc, char *argv[]) {

  int socket_fd;
  struct sockaddr_in server_addr;
  char msg[BUFFER_SIZE];

  printf("udp sender running.\n");

  // create socket
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd <= 0) {
    perror("Socket creation failed.\n");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(TARGET_PORT);
  if (inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr) <= 0) {
    perror("Invalid address / Address not supported.\n");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  // send to server
  while (1) {

    printf("enter a message: ");
    fflush(stdout);

    if (fgets(msg, sizeof(msg), stdin) == NULL) {
      break;
    }

    msg[strcspn(msg, "\n")] = '\0';

    if (strlen(msg) <= 0) {
      printf("Empty message, skipping...\n");
      continue;
    }

    ssize_t bytes =
        sendto(socket_fd, msg, strlen(msg), 0, (struct sockaddr *)&server_addr,
               sizeof(server_addr));
    if (bytes < 0) {
      perror("send to failed");
      close(socket_fd);
      exit(EXIT_FAILURE);
    }
  }

  return EXIT_SUCCESS;
}
