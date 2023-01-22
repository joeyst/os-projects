
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>

void read_from_file_descriptor(int file_descriptor) {

  // initially setting `number_of_bytes_read` to a value 
  // that is not 0 and is not -1. 
  int number_of_bytes_read = 1;
  char* text = malloc(2048 * sizeof(char));

  while (number_of_bytes_read != 0 && number_of_bytes_read != -1) {
    number_of_bytes_read = read(file_descriptor, text, 2048);
    write(1, text, number_of_bytes_read);
  }
  
  close(file_descriptor);
}

void read_file(char* file_to_read) {
  int file_descriptor = open(file_to_read, O_RDONLY);
  read_from_file_descriptor(file_descriptor);
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      read_file(argv[i]);
    }
  } else {
    read_from_file_descriptor(0);
  }
}
