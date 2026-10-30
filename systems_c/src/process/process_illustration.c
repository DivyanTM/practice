#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Process Programming illustration.\n");
  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork failed.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    printf("child : %d : executing :'ls -la'...\n", pid);

    char *args[] = {"contents_copy", "--help", NULL};
    execvp("./build/contents_copy", args);
    perror("Exec Failed.\n");
    exit(EXIT_FAILURE);
  } else {
    printf("Parent %d waiting for child %d to finish ..\n", getpid(), pid);

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
      printf("Parent : child finished with exit code %d.\n",
             WEXITSTATUS(status));
    }
  }

  return EXIT_SUCCESS;
}
