
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 128
#define MAX_CHARS 2048

int run_commands(char** command_args) {
  execvp(command_args[0], command_args);
}

int get_and_run_command() {
  printf("mush$ "); 
  // Allocating an array of strings that can hold 
  // the max number of arguments plus `NULL` to terminate. 
  char* command_args[MAX_ARGS + 1];
  int command_index = 0;

  // Taken from Exploration 3.1: 
  //////////////////////

  // Allocating a string that can store user input plus a 
  // `NULL` character to terminate. 
  char inp[MAX_CHARS + 1];

  fflush(stdout);
  // I looked it up and (as said here: https://stackoverflow.com/questions/1660228/does-fgets-always-terminate-the-char-buffer-with-0) 
  // `fgets()` appends a `NULL` character, so that is automatically handled. 
  fgets(inp, sizeof inp, stdin);

  char *token;

  if ((token = strtok(inp, " \t\n\r")) != NULL) {
      do {
          printf("Command index: %i", command_index);
          printf("Token: \"%s\"\n", token);
          command_args[command_index] = token;
          command_index++; 
      } while (((token = strtok(NULL, " \t\n\r")) != NULL) && command_index < (MAX_ARGS));
  }
  command_args[command_index] = NULL;

  for (int j = 0; j < command_index; j++) {
    printf("%s\n", command_args[j]);
  }

  //////////////////////
  run_commands(command_args);
}

int main() {
  while(1) {
    get_and_run_command();
  }
  
  return 1;
}
