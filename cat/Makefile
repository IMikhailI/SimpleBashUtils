CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

all: s21_cat clean_obj

s21_cat: s21_cat.o flags_cat.o output_cat.o
	$(CC) $(FLAGS) -o s21_cat s21_cat.o flags_cat.o output_cat.o

clean: clean_obj
	rm -rf s21_cat .clang-format s21_cat.log

clean_obj:
	rm -rf *.o

clang-i:
	cp ../../materials/build/scripts/.clang-format .clang-format
	clang-format -i *.c *.h

clang-n:
	cp ../../materials/build/scripts/.clang-format .clang-format
	clang-format -n *.c *.h

test:
	bash tests.sh

val:
	bash test_leaks.sh

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c
