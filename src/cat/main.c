#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "CatFlags.h"

typedef struct {
  bool numberNonBlank;
  bool markEndl;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool printNonPrintable;
} Flags;

Flags catReadFlags(int argc, char **argv) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {0, 0, 0, 0}};

  Flags flags = {0};
  int currentFlag = getopt_long(argc, argv, "benstvET", longOptions, NULL);

  while (currentFlag != -1) {
    switch (currentFlag) {
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
        flags.markEndl = true;
        flags.printNonPrintable = true;
        break;
      case 'n':
        flags.numberAll = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.tab = true;
        flags.printNonPrintable = true;
        break;
      case 'v':
        flags.printNonPrintable = true;
        break;
      case 'T':
        flags.tab = true;
        break;
      case 'E':
        flags.markEndl = true;
    }

    currentFlag = getopt_long(argc, argv, "benstvET", longOptions, NULL);
  }
  return flags;
}

void CatFile(FILE *file, Flags flags, const char *table[static 256]) {
  int c = 0;
  int last = '\n';
  bool sq = false;
  int line = 0;

  while (fread(&c, 1, 1, file) > 0) {
    if (last == '\n') {
      if (flags.squeeze && c == '\n') {
        if (sq) continue;
        sq = true;
      } else {
        sq = false;
      }

      if (flags.numberNonBlank) {
        if (c != '\n') {
          printf("%6i\t", ++line);
        }
      }

      else if (flags.numberAll)
        printf("%6i\t", ++line);
    }

    if (!*table[c])
      printf("%c", '\0');
    else
      printf("%s", table[c]);
    last = c;
  }
}

void Cat(int argc, char *argv[], Flags flags, const char *table[static 256]) {
  for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
       ++filename) {
    if (**filename == '-') continue;

    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    CatFile(file, flags, table);
    fclose(file);
  }
}

int main(int argc, char **argv) {
  Flags flags = catReadFlags(argc, argv);
  const char *table[256];

  CatSetTable(table);

  if (flags.markEndl) CatSetEndl(table);
  if (flags.tab) CatSetTab(table);
  if (flags.printNonPrintable) CatSetNonPrintable(table);

  Cat(argc, argv, flags, table);
}