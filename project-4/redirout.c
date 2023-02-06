#include <stdio.h> // printf, perror 
#include <stdlib.h> // exit 
#include <unistd.h> // fork, execlp, close 
#include <fcntl.h> // open 
#include <string.h>

#define MAX_CHARS_PER_READ 2048

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
  // Using `0666` permissions so anyone can write to file (and therefore rewrite it) 
  //  if it already exists, and read it. 
  pid_t file_descriptor = open(file_name, O_TRUNC | O_WRONLY | O_CREAT, 0666);

  // Creating the pipe 
  pid_t pfd[2];
  pipe(pfd);

  // Forking child process 
  pid_t pid = fork();

  // Running the command 
  if (pid == 0) {
    // wiring write end to `stdout` + closing write end + closing 
    //  file we're writing to in parent process 
    dup2(pfd[1], 1);
    close(pfd[1]);
    close(file_descriptor);
    // closing reading end 
    close(pfd[0]);
    // running the command 
    // Might need to derefence `argv` 
    char** command_and_args = argv + 2;
    execvp(command_and_args[0], command_and_args);
    close(1);
  }

  // Writing to file 
  else {
    // wiring read end to `stdin` + closing read end 
    dup2(pfd[0], 0);
    close(pfd[0]);
    // closing reading end 
    close(pfd[1]);
    // looping until nothing left to write 
    char read_data[MAX_CHARS_PER_READ];
    int bytes_to_read;
    while ((bytes_to_read = read(0, read_data, sizeof(read_data))) > 0) {
      write(file_descriptor, read_data, bytes_to_read);
    }
    close(file_descriptor);
  }

  return 0;
}
