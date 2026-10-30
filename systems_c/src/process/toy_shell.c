#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char command[256];

  // The shell runs forever until you tell it to stop
  while (1) {
    printf("toy-shell> ");

    // 1. Read user input
    if (fgets(command, sizeof(command), stdin) == NULL) {
      break;
    }

    // Remove the trailing newline character from hitting 'Enter'
    command[strcspn(command, "\n")] = 0;

    // Built-in command to kill the shell itself
    if (strcmp(command, "exit") == 0) {
      break;
    }

    // 2. We need to run a command, so we clone the shell
    pid_t pid = fork();

    if (pid < 0) {
      perror("Fork failed");
    } else if (pid == 0) {
      // --- CHILD PROCESS ---
      // The child sacrifices itself to become the command the user typed
      char *args[] = {command, NULL};
      execvp(command, args);

      // If execvp returns, the command didn't exist in the system
      printf("toy-shell: command not found: %s\n", command);
      exit(1);
    } else {
      // --- PARENT PROCESS (The Shell) ---
      // The shell survives, but goes to sleep until the child finishes
      wait(NULL);
    }
  }

  return 0;
}
