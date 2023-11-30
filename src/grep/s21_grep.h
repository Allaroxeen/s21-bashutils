#ifndef SRC_S21_GREP_H_
#define SRC_S21_GREP_H_

#include <stddef.h>
#include <stdio.h>

struct opts {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
    char file[BUFSIZ];
    char pattern[BUFSIZ];
};

// void grepfile(char* argv, struct opts);
int create_pattern(struct opts* grepopts);
int check_regs(char* str, struct opts grepopts);
int output(struct opts grepopts, char* argv);
void otheropts(struct opts grepopts, size_t ct, char* argv);
int check_onlyregs(struct opts grepopts, char* str, size_t ct, char* argv);
// int cur_regs(char* str, char* greppattern);

#endif  // SRC_S21_GREP_H_