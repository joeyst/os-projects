
#include <stdio.h>

int main() {
  int x = 12;
  float y = 3.14;
  char* s = "Hello, world!";

  printf("x is %i, y is %f\n", x, y);
  printf("%s\n", s);
  for (int i = 0; i < 5; i++) {
    printf("%i x 5 = %i\n", i, i * 5);
  }

  return 0;
}
