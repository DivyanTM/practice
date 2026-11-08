#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  printf("IPC : pipes basics\n");

  int fd[2];

  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  char message[] = "hello through pipe";
  char buffer[100] = {0};

  write(fd[1], message, strlen(message));
  read(fd[0], buffer, sizeof(buffer));

  printf("Received : %s\n", buffer);

  close(fd[0]);
  close(fd[1]);

  return EXIT_SUCCESS;
}
