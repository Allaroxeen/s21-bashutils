

#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    struct opts grepopts = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "", ""};
    for (int opt; (opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1;) {
        switch (opt) {
        case 'e':
            grepopts.e = 1;
            if (!strlen(grepopts.pattern)) {
                snprintf(grepopts.pattern, BUFSIZ, "%s", optarg);
            } else {
                snprintf(grepopts.pattern + strlen(grepopts.pattern),
                         BUFSIZ - strlen(grepopts.pattern), "|%s", optarg);
            }
            break;
        case 'i':
            grepopts.i = 1;
            break;
        case 'v':
            grepopts.v = 1;
            break;
        case 'c':
            grepopts.c = 1;
            break;
        case 'l':
            grepopts.l = 1;
            break;
        case 'n':
            grepopts.n = 1;
            break;
        case 'h':
            grepopts.h = 1;
            break;
        case 's':
            grepopts.s = 1;
            break;
        case 'f':
            grepopts.f = 1;
            snprintf(grepopts.file, BUFSIZ, "%s", optarg);
            break;
        case 'o':
            grepopts.o = 1;
            break;
        }
    }
    int error = 0;
    if (argc > optind && !grepopts.e && !grepopts.f) {
        snprintf(grepopts.pattern, BUFSIZ, "%s", argv[optind]);
        optind++;
    } else if (argc > optind && grepopts.f && strlen(grepopts.file)) {
        error = create_pattern(&grepopts);
    }
    // printf("strlen = %ld\n", strlen(grepopts.file));
    // printf("error = %d\n", error);
    if ((argc - optind) == 1) {
        grepopts.h = 1;
    }
    if ((!error) && (argc > optind)) {
        //  && (strlen(grepopts.pattern) != 0)
        while (argc > optind) {
            output(grepopts, argv[optind]);
            optind++;
        }
    }

    // for (; argc > optind; optind++) {
    //     printf("\n%d: %s", optind, argv[optind]);
    // }
    return 1;
}

int create_pattern(struct opts* grepopts) {
    int error = 0;
    FILE* ff;
    char str[BUFSIZ];
    ff = fopen(grepopts->file, "rt");
    if (ff) {
        if (fgets(str, sizeof(str), ff)) {
            if (strlen(str) > 1 && str[strlen(str) - 1] == '\n') {
                str[strlen(str) - 1] = '\0';
            }
            snprintf(grepopts->pattern, BUFSIZ, "%s", str);
        }
        while (fgets(str, sizeof(str), ff)) {
            if (strlen(str) > 1 && str[strlen(str) - 1] == '\n') {
                str[strlen(str) - 1] = '\0';
            }
            snprintf(grepopts->pattern + strlen(grepopts->pattern),
                     BUFSIZ - strlen(grepopts->pattern), "|%s", str);

            // printf("112: %s\n", str_buf);
        }
        fclose(ff);
    } else {
        if (!grepopts->h) {
            printf("grep: %s: No such file or directory\n", grepopts->file);
        }

        error = -1;
    }
    return error;
}

int check_regs(char* str, struct opts grepopts) {
    regex_t grepregs;
    int res;
    int result;
    if (grepopts.i) {
        res = regcomp(&grepregs, grepopts.pattern, REG_EXTENDED | REG_NEWLINE | REG_NOSUB | REG_ICASE);
    } else {
        res = regcomp(&grepregs, grepopts.pattern, REG_EXTENDED | REG_NEWLINE | REG_NOSUB);
    }
    if (res) {
        // if (!grepopts.s) {
        //     printf("grep: Failed, bad pattern\n");
        // }
        result = -1;
    } else {
        regmatch_t regmt;
        result = regexec(&grepregs, str, 1, &regmt, 0);
        regfree(&grepregs);
    }
    // printf("result = %d\n", result);
    return result;
}

int output(struct opts grepopts, char* argv) {
    FILE* f;
    int wexit = 0;
    int flag = 0;
    int check = 0;
    int error = 0;
    int res;
    char str[BUFSIZ];
    size_t cmline = 0;
    size_t ct = 0;
    f = fopen(argv, "rt");
    if (f) {
        error = 1;
        while (fgets(str, sizeof(str), f) && !wexit) {
            check = 0;
            if (strlen(str) > 1 && str[strlen(str) - 1] != '\n') {
                size_t k = strlen(str);
                str[k] = '\n';
                str[k + 1] = '\0';
            }
            ct++;
            if (grepopts.l) {
                res = check_regs(str, grepopts);
                if (!res) {
                    if (!grepopts.v) {
                        flag = 1;
                        wexit = 1;
                    }
                } else if (res != -1) {
                    if (grepopts.v) {
                        flag = 1;
                        wexit = 1;
                    }
                } else {
                    wexit = 1;
                }
            } else if (grepopts.c) {
                res = check_regs(str, grepopts);
                if (!res) {
                    if (!grepopts.v) {
                        cmline++;
                    }
                } else if (res == -1) {
                    wexit = 1;
                } else {
                    if (grepopts.v) {
                        cmline++;
                    }
                }
            } else {
                if (grepopts.o) {
                    if (!grepopts.v) {
                        res = check_onlyregs(grepopts, str, ct, argv);
                        if (res == -1) {
                            wexit = 1;
                        }
                    } else {
                        wexit = 1;
                    }
                } else {
                    res = check_regs(str, grepopts);
                    if (!res) {
                        if (!grepopts.v) {
                            otheropts(grepopts, ct, argv);
                            printf("%s", str);
                            if (str[(int)(strlen(str) - 1)] != '\n') {
                                check = 1;
                            }
                        }
                    } else if (res != -1) {
                        if (grepopts.v) {
                            otheropts(grepopts, ct, argv);
                            printf("%s", str);
                            if (str[(int)(strlen(str) - 1)] != '\n') {
                                check = 1;
                            }
                        }
                    } else {
                        wexit = 1;
                    }
                }
            }
        }
        if (!grepopts.l && grepopts.c) {
            if (!grepopts.h) {
                printf("%s:", argv);
            }
            printf("%ld\n", cmline);
        }
        if (check) {
            printf("\n");
        }
        if (flag == 1) {
            printf("%s\n", argv);
        }
    } else {
        if (!grepopts.s) {
            printf("grep: %s: No such file or directory\n", argv);
        }
    }
    return error;
}

int check_onlyregs(struct opts grepopts, char* str, size_t ct, char* argv) {
    regex_t grepregs;
    int res;
    int result;
    char* ptr_str = str;
    char* k = strchr(grepopts.pattern, '\n');
    // printf("pattern = %s\n", grepopts.pattern);
    while (k) {
        if (*(k + 1) == '|') {
            //         printf("flag 1");
            for (size_t i = 0; i < strlen(grepopts.pattern); i++) {
                *(k + i) = *(k + i + 2);
            }
            k = strchr(grepopts.pattern, '\n');
        } else if (*(k - 1) == '|') {
            *(k - 1) = '\0';
            *k = '\0';
            k = strchr(grepopts.pattern, '\n');
        } else {
            *k = '\0';
            k = strchr(grepopts.pattern, '\n');
        }
    }
    //  printf("pattern - %s\n", grepopts.pattern);
    if (grepopts.i) {
        res = regcomp(&grepregs, grepopts.pattern, REG_EXTENDED | REG_NEWLINE | REG_ICASE);
    } else {
        res = regcomp(&grepregs, grepopts.pattern, REG_EXTENDED | REG_NEWLINE);
    }
    if ((res && grepopts.s) || grepopts.pattern[0] == '\n' || !strlen(grepopts.pattern)) {
        result = -1;
    } else {
        regmatch_t regmt;
        // int k = 0;
        // if (!grepopts.h)
        //     k = 1;
        while (!regexec(&grepregs, ptr_str, 1, &regmt, 0)) {
            result = 0;
            otheropts(grepopts, ct, argv);
            printf("%.*s\n", (int)(regmt.rm_eo - regmt.rm_so), ptr_str + regmt.rm_so);
            ptr_str = ptr_str + (int)(regmt.rm_eo);
            // if (k) {
            //     grepopts.h = 1;
            // }
        }
        // if (k) {
        //     grepopts.h = 0;
        // }
        // if (!regexec(&grepregs, ptr_str, 1, &regmt, 0)) {
        //     result = 0;
        //     otheropts(grepopts, ct, argv);
        //     printf("%.*s\n", (int)(regmt.rm_eo - regmt.rm_so), ptr_str + regmt.rm_so);
        //     ptr_str = ptr_str + (int)(regmt.rm_eo);
        //     printf("ptr_str = %s\n", ptr_str);
        // }
        regfree(&grepregs);
    }
    return result;
}

void otheropts(struct opts grepopts, size_t ct, char* argv) {
    if (!grepopts.h) {
        printf("%s:", argv);
    }
    if (grepopts.n) {
        printf("%ld:", ct);
    }
}
