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

  // Opening file. I think we might've covered this already 
  //  but using `O_TRUNC` from docs here: https://man7.org/linux/man-pages/man2/open.2.html 
  pid_t file_descriptor = open(file_name, O_TRUNC);


  return 0;
}
