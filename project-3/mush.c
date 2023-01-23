
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

  // I looked it up and (as said here: https://stackoverflow.com/questions/1660228/does-fgets-always-terminate-the-char-buffer-with-0) 
  // `fgets()` appends a `NULL` character, so that is automatically handled. 
  fgets(inp, ((MAX_CHARS + 1) * sizeof(char)), stdin);

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
          command_args[command_index] = token;
          command_index++; 
      } while (((token = strtok(NULL, " \t\n\r")) != NULL) && (command_index < (MAX_ARGS)));
  }
  command_args[command_index] = NULL;
  return command_args;
}

char** get_command_args() {
  return get_list_of_args_from_string(get_user_input());
}

int run_commands(char** command_args) {
  execvp(command_args[0], command_args);
  perror("exec");
  exit(1);
}

int is_cd(char** command_args) {
  return (strcmp(command_args[0], "cd") == 0);
}

int run_cd(char** command_args) {
  return chdir(command_args[1]);
}

int is_cd_error(int cd_error) {
  return (cd_error == -1);
}

void print_cd_error() {
  perror("cd error:");
}

void handle_cd(char** command_args) {
  int cd_error = run_cd(command_args);
  if (is_cd_error(cd_error)) {
    print_cd_error();
  }
}

int is_exit(char** command_args) {
  return (strcmp(command_args[0], "exit") == 0);
}

int run_exit(char** command_args) {
  exit(0);
}

int is_exit_error(int exit_error) {
  return (exit_error == -1);
}

void print_exit_error() {
  perror("exit error:");
}

void handle_exit(char** command_args) {
  int exit_error = run_exit(command_args);
  if (is_exit_error(exit_error)) {
    print_exit_error();
  }
}

int is_child_process(pid_t process_id) {
  return (process_id == 0);
}

int main() {
  while(1) {
    char** command_args = get_command_args();

    if (is_cd(command_args)) {
      handle_cd(command_args);
    } else if (is_exit(command_args)) {
      handle_exit(command_args);
      exit(0);
    } else {
      pid_t process_id = fork();
      if (is_child_process(process_id)) {
        run_commands(command_args);
      } else {
        wait(NULL);
      }
    }
  }
  
  return 1;
}
