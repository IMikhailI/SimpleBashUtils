#include "s21_cat.h"

void output(Flags *arg, int argc, char *argv[]) {
  int count_line = 1;
  int count_empty_str = 1;
  char prev = '\n';
  for (int i = optind; i < argc; ++i) {
    FILE *f = fopen(argv[i], "r");
    if (f == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      continue;
    }
    char c;
    while ((c = fgetc(f)) != EOF) {
      if (c == '\n')
        count_empty_str++;
      else
        count_empty_str = 0;
      if (arg->flag_s && count_empty_str > 2) {
        continue;
      }
      if (arg->flag_b && c != '\n' && prev == '\n') {
        fprintf(stdout, "%6d\t", count_line++);
      }
      if (arg->flag_n && !arg->flag_b && prev == '\n') {
        fprintf(stdout, "%6d\t", count_line++);
      }
      if (arg->flag_t && c == '\t') {
        fprintf(stdout, "^I");
      } else {
        if (arg->flag_e && c == '\n') {
          fprintf(stdout, "$");
        }
        if (arg->flag_v) {
          processing_v_flag(c);
        } else {
          putchar(c);
        }
      }
      prev = c;
    }
    fclose(f);
  }
}

void processing_v_flag(unsigned char c) {
  if (c == 9 || c == 10 || (c >= 32 && c < 127)) {
    printf("%c", c);
  } else if (c == 127) {
    printf("^?");
  } else if (c >= 128 + 32) {
    printf("M-");
    (c < 128 + 127) ? printf("%c", c - 128) : printf("^?");
  } else {
    (c > 32) ? printf("M-^%c", c - 128 + 64) : printf("^%c", c + 64);
  }
}