
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>

int main(int argc, char *argv[]) {
  char* file_to_read = argv[1];
  int file_descriptor = open(file_to_read, O_RDONLY);
  char* text = malloc(2048 * sizeof(char));
  int number_of_bytes_read = read(file_descriptor, text, 2048);
  write(1, text, number_of_bytes_read);
  close(file_descriptor);
}
