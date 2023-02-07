#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define REG_EXT_NEW REG_EXTENDED | REG_NEWLINE

struct flags {
  int c, e, f, h, i, l, n, o, v, s, files, count_c, count_l, count_n;
  /*
  -e Шаблон
  -i Игнорирует различия регистра
  -v Инвертирует смысл поиска соответствий
  -c Выводит только количество совпадающих строк
  -l Выводит только совпадающие файлы
  -n Предваряет каждую строку вывода номером строки из файла ввода
  -h Выводит совпадающие строки, не предваряя их именами файлов
  -s Подавляет сообщения об ошибках о несущ. или нечит. файлах
  -f Получает регулярные выражения из файла
  -o Печатает только совпадающие (непустые) части совпавшей строки
  */
};

struct options {
  size_t count;
  char name[1024][1024];
};

void search_flag(int c, char** argv);
void input_file(const char* file_name);
void input_pttrn(char* str);
void output_file(FILE* file);

#endif