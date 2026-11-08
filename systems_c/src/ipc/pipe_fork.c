#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  printf("Pipe fork example\n");

  int fd[2];

  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();

  if (pid == -1) {
    perror("FORK");
    close(fd[0]);
    close(fd[1]);
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    close(fd[1]);

    char message[100] = {0};
    ssize_t n = read(fd[0], message, sizeof(message));

    if (n == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    printf("Received : %s\n", message);
    close(fd[0]);
  } else {
    close(fd[0]);
    char message[] = "hello from parent";
    if (write(fd[1], message, strlen(message)) == -1) {
      perror("write");
    }
    close(fd[1]);
    waitpid(pid, NULL, 0);
  }

  return EXIT_SUCCESS;
}
