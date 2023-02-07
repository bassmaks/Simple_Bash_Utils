TARGET = s21_grep
CC = gcc 
FLAGS = -Wall -Werror -Wextra -std=c11

$(TARGET) : s21_grep.o
	$(CC) $(FLAGS) s21_grep.o -o $(TARGET)

s21_grep.o : s21_grep.c
	$(CC) $(FLAGS) -c s21_grep.c -o s21_grep.o

clean : 
	rm -rf $(TARGET) *.o
