CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra
CLEAN=rm -Rf

uname_S := $(shell uname -s)
ifeq ($(uname_S), Linux)
	CHECK_LIBS=-lcheck -lm -lpthread -lsubunit -lrt
	LEAKS=valgrind s21_cat
else
	CHECK_LIBS=-lcheck -lm -lpthread
	LEAKS=CK_FORK=no leaks --atExit -- ./s21_cat
endif

SRC_DIR=.
TEST_DIR=../tests
SOURCES=*.c
HEADERS=*.h
OBJ=*.o
catpath=cat/*.c
greppath=grep/*.c
LIB_NAME=s21_math.a

all: clean test

s21_cat:
	$(CC) $(CFLAGS) *.c *.h -o s21_cat

clean:
	$(CLEAN) $(OBJ) *.a 
	$(CLEAN) s21_cat *.out log.txt

# для запуска тестов
test: s21_cat
	sh cat_test.sh

# проверки
lint:
	python3 ../../materials/linters/cpplint.py --recursive --extensions=c ../../src/cat

cppcheck:
	cppcheck --std=c11 --enable=all --suppress=missingIncludeSystem  --suppress=unusedFunction --suppress=invalidPointerCast ./

leaks:
	$(LEAKS)

build: 
	$(CC) *.c -g