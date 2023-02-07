TARGET = s21_cat
CC = gcc 
FLAGS = -Wall -Werror -Wextra -std=c11

$(TARGET) : s21_cat.o
	$(CC) $(FLAGS) s21_cat.o -o $(TARGET)

s21_cat.o : s21_cat.c
	$(CC) $(FLAGS) -c s21_cat.c -o s21_cat.o

clean : 
	rm -rf $(TARGET) *.o
