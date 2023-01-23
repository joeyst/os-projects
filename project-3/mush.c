
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  while(1) {
    printf("mush$ "); 
    char* command_args[256];
    int command_index = 0;

    // Taken from Exploration 3.1: 
    //////////////////////

    char inp[2048];

    fflush(stdout);
    fgets(inp, sizeof inp, stdin);

    char *token;

    if ((token = strtok(inp, " \t\n\r")) != NULL) {
        do {
            printf("Token: \"%s\"\n", token);
            command_args[command_index] = token;
            command_index++; 
        } while ((token = strtok(NULL, " \t\n\r")) != NULL);
    }

    for (int j = 0; j < command_index; j++) {
      printf("%s\n", command_args[j]);
    }

    //////////////////////

  }
  
  return 1;
}
