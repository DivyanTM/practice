#include <asm-generic/errno-base.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Kill System call\n");

  int signal, kill_return = -1;

  if (argc != 3 || strcmp(argv[1], "--help") == 0) {
    printf("Usage : %s sig_num pid\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int pid = atoi(argv[2]);

  if (pid <= 0) {
    perror("Invalid Process ID");
    exit(EXIT_FAILURE);
  }

  signal = atoi(argv[1]);
  if (signal < 0) {
    perror("Invalid signal");
    exit(EXIT_FAILURE);
  }

  kill_return = kill(pid, signal);

  if (kill_return == -1) {
    perror("Failed to kill");
    exit(EXIT_FAILURE);
  } else {
    if (kill_return == 0) {
      printf("Process exists and we can send signal\n");
    } else {
      if (errno == EPERM) {
        printf("Process exists but we dont have enough permissions to send the "
               "signals\n");
      } else if (errno == ESRCH) {
        printf("process does not exists\n");
      } else {
        perror("Err exit");
        exit(EXIT_FAILURE);
      }
    }
  }

  return EXIT_SUCCESS;
}
