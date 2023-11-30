#ifndef SRC_S21_CAT_H_
#define SRC_S21_CAT_H_

enum flags {
    number_nonblank,
    number,
    squeeze_blank,
    show_tabs,
    show_ends,
    show_nonprinting,
    all
};

int flags_current[all] = {0};

void printfile(const char* file, const char* NP);
void flagchange(enum flags curr_flag);
void charchange(const char c, const char* NP);

#endif  // SRC_S21_CAT_H_