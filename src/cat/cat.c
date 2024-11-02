#include "cat.h"

int main(int argc, char *argv[]) {
  Flag flags = {0};

  catFlags(argc, argv, &flags);

  while (optind < argc) {
    catFile(argv, &flags);
    optind++;
  }

  return 0;
}

void catFlags(int argc, char *argv[], Flag *flags) {
  struct option longOptions[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  int currentFlag = getopt_long(argc, argv, "benstvET", longOptions, NULL);

  while (currentFlag != -1) {
    switch (currentFlag) {
      case 'b':
        flags->b = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      default:
        printf("error\n");
        break;
    }
    currentFlag = getopt_long(argc, argv, "benstvET", longOptions, NULL);
  }
  if (flags->b && flags->n) flags->n = 0;
}

void catFile(char *argv[], Flag *flags) {
  FILE *fp;
  fp = fopen(argv[optind], "r");

  if (fp != NULL) {
    int str_count = 1;
    int empty_str_count = 0;
    int last = '\n';

    while (!feof(fp)) {
      int c = fgetc(fp);
      if (c == EOF) break;
      if (flags->s && c == '\n' && last == '\n') {
        empty_str_count++;
        if (empty_str_count > 1) {
          continue;
        }
      } else {
        empty_str_count = 0;
      }

      if (last == '\n' && ((flags->b && c != '\n') || flags->n))
        printf("%6d\t", str_count++);
      if (flags->t && c == '\t') {
        printf("^");
        c = 'I';
      }
      if (flags->e && c == '\n') printf("$");
      if (flags->v) {
        if ((c >= 0 && c < 9) || (c > 10 && c < 32) || (c > 126 && c <= 160)) {
          printf("^");
          if (c > 126) {
            c -= 64;
          } else {
            c += 64;
          }
        }
      }
      printf("%c", c);
      last = c;
    }
    fclose(fp);
  } else {
    printf("Error\n");
    exit(1);
  }
}
