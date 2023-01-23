
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 128
#define MAX_CHARS 2048

char* get_user_input() {
  printf("mush$ "); 
  // Allocating a string that can store user input plus a 
  // `NULL` character to terminate. 
  char* inp = calloc(MAX_CHARS + 1, sizeof(char));
  fflush(stdout);

  // I looked it up and (as said here: https://stackoverflow.com/questions/1660228/does-fgets-always-terminate-the-char-buffer-with-0) 
  // `fgets()` appends a `NULL` character, so that is automatically handled. 
  fgets(inp, sizeof inp, stdin);

  return inp;
}

char** get_list_of_args_from_string(char* user_input) {
  // Allocating an array of strings that can hold 
  // the max number of arguments plus `NULL` to terminate. 
  char** command_args = calloc(MAX_ARGS + 1, sizeof(char*));
  int command_index = 0;

  // Taken from Exploration 3.1: 
  //////////////////////
  char *token;

  if ((token = strtok(user_input, " \t\n\r")) != NULL) {
      do {
          printf("Command index: %i", command_index);
          printf("Token: \"%s\"\n", token);
          fflush(stdout);
          command_args[command_index] = token;
          command_index++; 
      } while (((token = strtok(NULL, " \t\n\r")) != NULL) && command_index < (MAX_ARGS));
  }
  command_args[command_index] = NULL;

  for (int j = 0; j < command_index; j++) {
    printf("%s\n", command_args[j]);
    fflush(stdout);
  }

  return command_args;
}

char** get_command_args() {
  return get_list_of_args_from_string(get_user_input());
}

int run_commands(char** command_args) {
  execvp(command_args[0], command_args);
}

int main() {
  while(1) {
    char** command_args = get_command_args();
    run_commands(command_args);
  }
  
  return 1;
}
