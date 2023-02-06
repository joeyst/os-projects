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

  // Creating the pipe 
  pid_t pfd[2];
  pipe(pfd);

  // Running the command 
  if (file_descriptor == 0) {
    // wiring write end to `stdout` + closing write end 
    dup2(pfd[1], 1);
    close(pfd[1]);
    // closing reading end 
    close(pfd[0]);
    // running the command 
    // Might need to derefence `argv` 
    char** command_and_args = argv + (sizeof(char*) * 2);
    execvp(command_and_args[0], command_and_args);
  }

  // Writing to file 
  else {
    // wiring read end to `stdin` + closing read end 
    dup2(pfd[0], 0);
    close(pfd[0]);
  }

  return 0;
}
