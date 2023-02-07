#include "s21_grep.h"

struct flags flag = {0};
struct options pttrn = {0};
struct options txt_file = {0};

int main(int argc, char** argv) {
  int c;
  if (argc < 3 && !flag.s) {
    fprintf(stderr,
            "s21_grep: option requires an argument -%s\n"
            "usage: s21_grep [-cefhilnovs] [-A num] [-B num] [-C[num]]\n"
            "\t[-e pattern] [-f file] [--binary-files=value] [--color=when]\n"
            "\t[--context[=num]] [--directories=action] [--label] "
            "[--line-buffered]\n"
            "\t[--null] [pattern] [file ...]\n",
            argv[optind]);

  } else {
    while ((c = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != -1)
      search_flag(c, argv);
    if (!flag.e && !flag.f) strcpy(pttrn.name[pttrn.count++], argv[optind++]);
    flag.files = argc - optind;
    while (optind < argc) input_file(argv[optind++]);
    return 0;
  }
}

void search_flag(int c, char** argv) {
  if (c == '?' && !flag.s) {
    fprintf(stderr,
            "s21_grep: option requires an argument -%s\n"
            "usage: s21_grep [-cefhilnovs] [-e pattern] [-f file] ...\n",
            argv[optind]);
  } else {
    size_t res = 0;
    for (const char* str = "cefhilnovs"; str[res] != (char)c; res++) {
    }
    *(&flag.c + res) = 1;
    if (c == 'e') strcpy(pttrn.name[pttrn.count++], optarg);
    if (c == 'f') {
      FILE* file = fopen(optarg, "r");
      if (file) {
        for (char ch = getc(file); !feof(file);
             pttrn.count++, ch = getc(file)) {
          size_t pos = 0;
          pttrn.name[pttrn.count][pos] = ch;
          while (ch != '\n' && !feof(file)) {
            pttrn.name[pttrn.count][pos++] = ch;
            ch = getc(file);
          }
        }
        fclose(file);
      } else {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", optarg);
      }
    }
  }
}

void input_file(const char* file_name) {
  FILE* file = fopen(file_name, "r");
  if (file) {
    strcpy(txt_file.name[txt_file.count++], file_name);
    output_file(file);
    fclose(file);
  } else if (!flag.s) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", file_name);
  }
}

void output_file(FILE* file) {
  char str[1024] = "";
  flag.count_c = flag.count_l = flag.count_n = 0;
  for (char c = getc(file); !feof(file); c = getc(file)) {
    flag.count_n++;
    size_t pos = 0;
    str[pos] = c;
    while (c != '\n' && !feof(file)) {
      str[pos++] = c;
      c = getc(file);
    }
    str[pos] = '\0';
    input_pttrn(str);
    if (flag.count_l == 1) break;
  }
  if (flag.o && !flag.l && !flag.v && !flag.c) return;
  if (flag.c) {
    if (flag.files > 1 && !flag.h)
      printf("%s:", txt_file.name[txt_file.count - 1]);
    printf("%d\n", flag.count_c);
  }
  if (flag.l && flag.count_l) printf("%s\n", txt_file.name[txt_file.count - 1]);
}

void input_pttrn(char* str) {
  size_t res = 0, first_file = 0, first_num = 0;
  regmatch_t pmatch[1] = {0};
  regex_t regex = {0};

  for (size_t current = 0; current < pttrn.count; current++) {
    if (regcomp(&regex, pttrn.name[current],
                flag.i ? REG_EXT_NEW | REG_ICASE : REG_EXT_NEW))
      continue;
    if (flag.o && !flag.l && !flag.v && !flag.c) {
      while (!regexec(&regex, str, 1, pmatch, 0)) {
        if (first_file++ == 0)
          if (flag.files > 1 && !flag.h)
            printf("%s:", txt_file.name[txt_file.count - 1]);
        if (flag.n && first_num++ == 0) printf("%d:", flag.count_n);
        printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
               str + pmatch[0].rm_so);
        str += pmatch[0].rm_eo;
      }
    } else if (regexec(&regex, str, 0, pmatch, 0) == flag.v) {
      res++;
    }
    regfree(&regex);
  }
  if (res == pttrn.count || (res && !flag.v)) {
    if (flag.c) flag.count_c++;
    if (flag.l) flag.count_l = 1;
    if (!flag.c && !flag.l) {
      if (flag.files > 1 && !flag.h)
        printf("%s:", txt_file.name[txt_file.count - 1]);
      if (flag.n) printf("%d:", flag.count_n);
      printf("%s\n", str);
    }
  }
}
