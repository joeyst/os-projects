#include <stdio.h> // printf, perror 
#include <stdlib.h> // exit 
#include <unistd.h> // fork, execlp, close 
#include <fcntl.h> // open 
#include <string.h>

int main(int argc, char *argv[]) {
  // Validating command-line arguments 
  if (argc < 3) {
    printf("Too few arguments. Needs at least file name and command to run.");
    fflush(stdout);
    exit(1);
  }

  char* file_name = strdup(argv[1]);
  printf("file name: %s", file_name);
  fflush(stdout);

  return 0;
}
