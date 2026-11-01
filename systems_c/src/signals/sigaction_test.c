#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t stop = 0;

void handle_signal(int sig) {
  if (sig == SIGINT) {
    stop = 1;
  }
}

int main(void) {
  printf("sigaction test program.\n");
  struct sigaction sa = {0};
  sa.sa_handler = handle_signal;

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction");
    return 1;
  }

  while (!stop) {
    printf("Running...\n");
    if (stop) {
      printf("SIGINT Received\n");
    }
    sleep(2);
  }

  printf("Signal received shutting down gracefully...\n");

  return EXIT_SUCCESS;
}
