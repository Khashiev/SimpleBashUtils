#define _GNU_SOURCE

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int regex_flag;
  bool invert;
  bool count;
  bool filesMatch;
  bool numberLine;
  bool printMatched;
} Flags;

Flags grepReadFlags(int argc, char **argv) {
  Flags flags = {0, false, false, false, false, false};
  int currentFlag = getopt(argc, argv, "eivclno");

  while (currentFlag != -1) {
    switch (currentFlag) {
      case 'e':
        flags.regex_flag |= REG_EXTENDED;
        break;
      case 'i':
        flags.regex_flag |= REG_ICASE;
        break;
      case 'v':
        flags.invert = true;
        break;
      case 'c':
        flags.count = true;
        break;
      case 'l':
        flags.filesMatch = true;
        break;
      case 'n':
        flags.numberLine = true;
        break;
      case 'o':
        flags.printMatched = true;
        break;
    }

    currentFlag = getopt(argc, argv, "eivclno");
  }
  return flags;
}

void GrepCount(FILE *file, char const *filename, Flags flags, regex_t *preg,
               int count_file) {
  (void)filename;  // temp
  (void)flags;     // temp
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;

  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      count++;
    }
  }

  if (count_file >= 2)
    printf("%s:%i\n", filename, count);
  else
    printf("%i\n", count);

  free(line);
}

void GrepFile(FILE *file, Flags flags, regex_t *preg, char *filename) {
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;

  while (getline(&line, &length, file) > 0) {
    count++;
    if (flags.invert) {
      if (regexec(preg, line, 1, &match, 0)) {
        if (flags.printMatched)
          ;
        else {
          if (flags.numberLine)
            printf("%s:%i:%s", filename, count, line);
          else
            printf("%s", line);
        }
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        if (flags.printMatched) {
          if (flags.numberLine)
            printf("%s:%i:%.*s\n", filename, count, match.rm_eo - match.rm_so,
                   line + match.rm_so);
          else
            printf("%.*s\n", match.rm_eo - match.rm_so, line + match.rm_so);
        } else {
          if (flags.numberLine)
            printf("%i:%s", count, line);
          else
            printf("%s", line);
        }
      }
    }
  }

  free(line);
}

void Grep(int argc, char *argv[], Flags flags) {
  char **pattern = &argv[1];
  char **end = &argv[argc];
  int count = 0;
  regex_t preg_storage;
  regex_t *preg = &preg_storage;

  for (; pattern != end && pattern[0][0] == '-'; pattern++)
    ;

  if (pattern == end) {
    fprintf(stderr, "no pattern\n");
    exit(1);
  }

  if (regcomp(preg, *pattern, flags.regex_flag)) {
    fprintf(stderr, "failed to compile regex\n");
    exit(1);
  }

  for (char **filename = pattern + 1; filename != end; ++filename) {
    if (**filename == '-') continue;
    count++;

    if (count >= 2) break;
  }

  for (char **filename = pattern + 1; filename != end; ++filename) {
    if (**filename == '-') continue;

    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }

    if (flags.count) {
      GrepCount(file, *filename, flags, preg, count);
    } else {
      GrepFile(file, flags, preg, *filename);
    }

    fclose(file);
  }
}

int main(int argc, char **argv) {
  Flags flags = grepReadFlags(argc, argv);
  Grep(argc, argv, flags);
}
