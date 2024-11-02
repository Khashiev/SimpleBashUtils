#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_01 "s21_grep: %s: No such file or directory\n"

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int countFiles;
  int numberLine;
  int countMatches;
} options;

int grepFlags(int argc, char **argv, options *config, char **template);
void grepFile(char *name, options config, char *tmpl);

void printMainData(char *str, options *options, char *tmpl, char *name);
void printfAuxData(options config, char *path);

void setupConfig(options *config, int argc);
void setConfigE(options *config, char **template, char *optarg);
void *increaseLengthStr(void *str, int size);
int createTemplate(char **str, char *optarg);
void addTemplate(char **str, char *optarg);

#endif
