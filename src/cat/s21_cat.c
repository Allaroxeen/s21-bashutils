#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    while (1) {
        int c;
        int option_index = 0;
        static struct option cat_options[] = {
            {"number-nonblank", 0, NULL, 'b'},
            {"number", 0, NULL, 'n'},
            {"squeeze-blank", 0, NULL, 's'},  // 1
            {"show-tabs", 0, NULL, 'T'},
            {NULL, 0, NULL, 'e'},
            {"show-ends", 0, NULL, 'E'},
            {"show-nonprinting", 0, NULL, 'v'},
            {NULL, 0, NULL, 't'},
            {NULL, 0, NULL, 0},
        };
        c = getopt_long(argc, argv, "bnsTeEvt", cat_options, &option_index);

        if (c == -1) {
            break;
        }
        switch (c) {
        case 'b':
            flagchange(number_nonblank);
            break;
        case 'n':
            flagchange(number);
            break;
        case 's':
            flagchange(squeeze_blank);
            break;
        case 'T':
            flagchange(show_tabs);
            break;
        case 'e':
            flagchange(show_nonprinting);
            flagchange(show_ends);
            break;
        case 'E':
            flagchange(show_ends);
            break;
        case 'v':
            flagchange(show_nonprinting);
            break;
        case 't':
            flagchange(show_nonprinting);
            flagchange(show_tabs);
            break;
        }
    }
    char NP[31];
    for (int i = 0; i < 31; i++) {
        if ((i == 8) || (i == 9)) {
            NP[i] = 27;
        } else {
            NP[i] = (i + 1);
        }
    }
    if (optind == argc) {
        printfile("-", NP);
    }
    while (optind < argc) {
        printfile(argv[optind++], NP);
    }
    return 0;
}

void printfile(const char* file, const char* NP) {
    FILE* f;
    char read;
    if (strcmp(file, "-") == 0) {
    } else if ((f = fopen(file, "rt")) == NULL) {
        perror(file);
    } else {
        while (!feof(f) && !ferror(f)) {
            read = fgetc(f);
            if (read != '\xff') {
                charchange(read, NP);
            }
        }
        if (ferror(f)) {
            perror(file);
        }
        fclose(f);
    }
}

void charchange(const char c, const char* NP) {
    static int counter_ch = 1;
    static int counter = 1;
    static int previous = 1;
    static int np = 0;
    static char buf;
    if (counter == 1 && flags_current[number]) {
        if (c == '\n' && (!flags_current[number_nonblank])) {
            printf("%6d\t", counter);
            counter++;
        }
    }
    if (c == '\n') {
        if (flags_current[squeeze_blank]) {
            if (!previous && (buf == '\n' || counter_ch == 1)) {
                previous = 1;
                np = 0;
            } else if (previous) {
                (buf = '\0');
                np = 1;
            }
        }
        if ((buf == '\n') && (flags_current[number])) {
            if (!flags_current[number_nonblank]) {
                printf("%6d\t", counter);
                counter++;
            }
        }
        if (counter_ch == 1 || buf != '\0') {
            buf = c;
            np = 0;
        }
        if (flags_current[show_ends] && buf == '\n') {
            printf("$");
        }
    } else {
        if (((buf == '\n') || (previous)) && (flags_current[number] || flags_current[number_nonblank])) {
            printf("%6d\t", counter);
            counter++;
        }
        buf = c;
        previous = 0;
        np = 0;
        if (flags_current[show_nonprinting]) {
            if (strchr(NP, c)) {
                printf("^");
                buf = c + 64;
            } else if (c == '\0') {
                printf("^");
                buf = c + 64;
            } else if (c == 127) {
                printf("^");
                buf = '?';
            }
        }
        if (flags_current[show_tabs]) {
            previous = 0;
            if (c == 9) {
                printf("^");
                buf = c + 64;
            }
        }
    }
    if (c != '\xff' && !np) {
        printf("%c", buf);
    }
    counter_ch++;
}

void flagchange(enum flags curr_flag) {
    flags_current[curr_flag] = 1;
}
