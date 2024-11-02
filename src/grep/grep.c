#include "grep.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    char *temp = NULL;
    options config = {0};

    for (int x = grepFlags(argc, argv, &config, &temp); x < argc; x += 1)
      grepFile(argv[x], config, temp);

    if (temp != NULL) free(temp);
  }

  return 0;
}

int grepFlags(int argc, char **argv, options *config, char **template) {
  for (int sym = 0; (sym = getopt(argc, argv, "e:ivclnhsf:o")) != (-1);) {
    switch (sym) {
      case 'i':
        config->i = 1;
        break;
      case 'v':
        config->v = 1;
        break;
      case 'c':
        config->c = 1;
        break;
      case 'l':
        config->l = 1;
        break;
      case 'n':
        config->n = 1;
        break;
      case 'h':
        config->h = 1;
        break;
      case 's':
        config->s = 1;
        break;
      case 'e':
        setConfigE(config, template, optarg);
        break;
    }
  }
  if ((config->e || config->f) == 0) {
    createTemplate(template, argv[optind]);
    optind += 1;
  }
  setupConfig(config, argc);
  return optind;
}

void grepFile(char *path, options config, char *template) {
  FILE *file = fopen(path, "r");
  if (file != NULL) {
    for (char sym = '0'; (sym = getc(file)) != EOF;) {
      char *line = calloc(256, 1);
      config.numberLine += 1;
      int length = 0;

      for (line[length] = '\0'; sym != EOF && sym != '\n'; sym = getc(file)) {
        line[length] = sym;
        line[length += 1] = '\0';
        if (length % 255 == 0) line = increaseLengthStr(line, length + 256);
      }

      printMainData(line, &config, template, path);
      free(line);
    }
    printfAuxData(config, path);
    fclose(file);
  } else if (config.s == 0) {
    fprintf(stderr, ERROR_01, path);
  }
}

void printfAuxData(options config, char *path) {
  if (config.c) {
    if (config.l) {
      config.countFiles > 1 ? printf("%s:1\n", path) : printf("1\n");
    } else {
      if (config.countFiles > 1) printf("%s:", path);
      printf("%i\n", config.countMatches);
    }
  }
  if (config.l && config.countMatches) printf("%s\n", path);
}

void printMainData(char *line, options *config, char *template, char *path) {
  regex_t regex;
  if (regcomp(&regex, template, config->i ? REG_ICASE : REG_EXTENDED) == 0) {
    if (regexec(&regex, line, 0, NULL, 0) == config->v) {
      config->countMatches += 1;
      if ((config->c || config->l) == 0) {
        if (config->countFiles > 1 && !(config->h)) printf("%s:", path);
        if (config->n) printf("%i:", config->numberLine);
        printf("%s\n", line);
      }
    }
    regfree(&regex);
  }
}

void setupConfig(options *config, int argc) {
  config->countFiles = argc - optind;
}

void setConfigE(options *config, char **template, char *optarg) {
  if (!(config->e))
    config->e = createTemplate(template, optarg);
  else
    addTemplate(template, optarg);
}

int createTemplate(char **str, char *optarg) {
  *str = calloc(strlen(optarg) + 1, 1);
  if (*str) strcpy(*str, optarg);
  return str ? 1 : 0;
}

void addTemplate(char **str, char *optarg) {
  *str = increaseLengthStr(*str, strlen(*str) + strlen(optarg) + 2);
  if (*str) strcat(strcat(*str, "|"), optarg);
}

void *increaseLengthStr(void *str, int size) {
  char *aux = realloc(str, size);
  return aux;
}
