#include "wish.h"

char g_errorMessage[32] = "An error has occurred\n";

int main(int argc, char *argv[]) {
  if (argc == 1) {
    run_interactive();
  } else if (argc == 2) {
    run_batch(argv[1]);
  } else if (argc > 2) {
    write(STDERR_FILENO, g_errorMessage, strlen(g_errorMessage));
    return 1;
  }
  return 0;
}

void run_batch(char *fileName) {}
void run_interactive() {}
