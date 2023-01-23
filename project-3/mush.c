
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  while(1) {
    printf("mush$ "); 

    // Taken from Exploration 3.1: 
    //////////////////////

    char inp[2048];

    fflush(stdout);
    fgets(inp, sizeof inp, stdin);

    char *token;

    if ((token = strtok(inp, " \t\n\r")) != NULL) {
        do {
            printf("Token: \"%s\"\n", token);
        } while ((token = strtok(NULL, " \t\n\r")) != NULL);
    }

    //////////////////////

  }
  
  return 1;
}
