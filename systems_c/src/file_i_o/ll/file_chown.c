#include <stdio.h>
#include <unistd.h>

int main() {
  if (chown("test.txt", -1, 1000) < 0) {
    perror("chown failed");
  } else {
    printf("Group ownership updated.\n");
  }

  return 0;
}
