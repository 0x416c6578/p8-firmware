#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Too few args\n");
    return 1;
  }
  int numChars = 0;
  while (argv[1][numChars] != 0) numChars++;
  printf("%i\n", numChars * atoi(argv[2]) * 5 + (numChars - 1) * atoi(argv[2]));
  return 0;
}
