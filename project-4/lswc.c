#include <stdio.h> // printf, perror 
#include <stdlib.h> // exit 
#include <unistd.h> // fork, execlp, close 

int main(int argc, char* argv[]) {
  // validation of command-line args + ensuring passing in directory name 
  if (argc > 2) {
    printf("Too many arguments. Use none to get number of files in current directory, or use one argument to specify directory to check.");
    fflush(stdout);
    exit(1);
  }

  // piping + forking 
  int pfd[2];
  pipe(pfd);
  pid_t pid = fork();

  // `wc`  
  if (pid == 0) {
    // wiring read end to `stdin` + closing read end 
    dup2(pfd[0], 0);
    close(pfd[0]);
    // closing wiring end 
    close(pfd[1]);
    // running `wc` command 
    execlp("wc", "wc", "-l", NULL);
  }

  // `ls` 
  else {
    // wiring write end to `stdout` + closing write end  
    dup2(pfd[1], 1);
    close(pfd[1]);
    // closing reading end 
    close(pfd[0]);
    // running `ls` command 
    if (argc == 2) {
      execlp("ls", "ls", "-1a", argv[1], NULL);
    }
    else {
      execlp("ls", "ls", "-1a", NULL);
    }
  }

  return 0;
}
