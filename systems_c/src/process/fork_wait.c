#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  printf("Fork wait example.\n");
  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    printf("Child process started.\n");
    sleep(3);
    printf("Child process finished.\n");
    exit(EXIT_SUCCESS);
  }

  printf("Waiting for the child\n");
  if (waitpid(pid, NULL, 0) == -1) {
    perror("waitpid");
    return -1;
  }

  printf("Child process terminated\n");

  return EXIT_SUCCESS;
}
