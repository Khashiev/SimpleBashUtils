all: s21_cat s21_grep

s21_cat: 
	gcc -Wall -Werror -Wextra cat/main.c -o cat/s21_cat

s21_grep:
	gcc -Wall -Werror -Wextra grep/main.c -o grep/s21_grep

clean:
	rm -rf cat/s21_cat grep/s21_grep

rebuild: 
	clean all
