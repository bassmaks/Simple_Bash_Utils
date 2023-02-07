#include "s21_cat.h"

int main(int argc, char *argv[]) {
  options opt = {0};
  Parser(argc, argv, &opt);
  Input(argc, argv, opt);
  return 0;
}

void Input(int argc, char *argv[], options opt) {
  for (int i = optind; i < argc; i++) {
    FILE *f = fopen(argv[i], "r");
    if (f) {
      Output(f, opt);
      fclose(f);
    } else {
      perror(argv[i]);
    }
  }
}

void Parser(int argc, char *argv[], options *opt) {
  int c = 0, option_index = 0;
  static struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };

  while ((c = getopt_long(argc, argv, "+benstvTE", long_options,
                          &option_index)) != -1) {
    switch (c) {
      case 'b':
        opt->b = 1;
        break;
      case 'e':
        opt->e = 1;
        opt->v = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 't':
        opt->t = 1;
        opt->v = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'T':
        opt->t = 1;
        break;
      case 'E':
        opt->e = 1;
        break;
      default:
        perror("Usage: s21_cat [-benstvTE] [file...]\n");
        exit(EXIT_FAILURE);
        break;
    }
  }
}

void Output(FILE *file, options opt) {
  int empty_str = 0;
  int count_str = 1;
  int mean_flag = 1;
  int meaning = 1;
  int nexter = '\n';

  while (meaning) {
    int ch = getc(file);
    if (ch == EOF) {
      meaning = 0;
    } else {
      if (opt.s && ch == '\n' &&  // сжатие нескольких пустых строк / флаг -s
          nexter == '\n') {
        empty_str++;
        if (empty_str > 1) {
          mean_flag = 0;
        }
      } else {
        mean_flag = 1;
        empty_str = 0;
      }
      if (mean_flag) {
        if (((ch != '\n' && opt.b) || (opt.n && !opt.b)) &&
            nexter == '\n') {  // нумерация строк / флаги -b -n
          printf("%6d\t", count_str);
          count_str++;
        }
        if ((opt.t) && ch == '\t') {  // отображение табов как ^I / влаги -t -T
          printf("^");
          ch = 'I';
        }
        if ((opt.v && ch >= 0 && ch < 32) &&
            (ch != 9 && ch != 10)) {  // флаг -v
          printf("^");
          ch += 64;
        }
        if (ch == 127 && opt.v == 1)  // дополнение к флагу -v
        {
          printf("^");
          ch = '?';
        }
        if ((opt.e) && ch == '\n')
          printf("$");  // конец строки как $ / флаги -e -E
        printf("%c", ch);
        nexter = ch;
      }
    }
  }
}
