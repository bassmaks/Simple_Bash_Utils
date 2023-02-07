#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int s;
  int t;
  int n;
  int v;
} options;

void Parser(int argc, char *argv[], options *opt);
void Input(int argc, char *argv[], options opt);
void Output(FILE *file, options opt);

#endif
