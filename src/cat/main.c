#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  if (argc == 2) {
    FILE* ptr;
    char ch;
    ptr = fopen(argv[1], "r");

    if (NULL == ptr) {
      printf("file can't be opened \n");
    }

    do {
      ch = fgetc(ptr);

      if ((int)ch < 0 || (int)ch > 126) continue;

      printf("%c", ch);
    } while (ch != EOF);

    fclose(ptr);

  } else if (argc == 3) {
    FILE* ptr;
    char ch;
    ptr = fopen(argv[2], "r");

    if (NULL == ptr) {
      printf("file can't be opened \n");
    }

    int counter = 1;
    printf("%6d  ", counter);
    counter++;

    do {
      ch = fgetc(ptr);

      if ((int)ch < 0 || (int)ch > 126) continue;

      printf("%c", ch);

      if (ch == '\n') {
        printf("%6d  ", counter);
        counter++;
      }
    } while (ch != EOF);

    fclose(ptr);
  } else {
    printf("0");
  }

  return 0;
}