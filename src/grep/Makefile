CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra
CLEAN=rm -Rf

uname_S := $(shell uname -s)
ifeq ($(uname_S), Linux)
	CHECK_LIBS=-lcheck -lm -lpthread -lsubunit -lrt
	LEAKS=valgrind s21_grep
else
	CHECK_LIBS=-lcheck -lm -lpthread
	LEAKS=CK_FORK=no leaks --atExit -- ./s21_grep
endif

all: clean test

s21_grep:
	$(CC) $(CFLAGS) *.c *.h -o s21_grep

clean:
	$(CLEAN) s21_grep *.out log.txt

# для запуска тестов
test: s21_grep
	sh grep_test.sh

# проверки
lint:
	python3 ../../materials/linters/cpplint.py --recursive --extensions=c ../../src/grep

cppcheck:
	cppcheck --std=c11 --enable=all --suppress=missingIncludeSystem  --suppress=unusedFunction --suppress=invalidPointerCast ./

leaks: s21_grep
	$(LEAKS)

build: 
	$(CC) *.c -g