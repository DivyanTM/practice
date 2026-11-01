#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_signal(int sig) {
  printf("\nSIGINT received\n");
  printf("received signal number : %d\n", sig);
}

int main(int argc, char *argv[]) {
  printf("Signal Handling test program\n");

  signal(SIGINT, handle_signal);

  while (1) {
    printf("running...\n");
    sleep(2);
  }

  return EXIT_SUCCESS;
}
