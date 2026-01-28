#ifndef S21_GREP_H
#define S21_GREP_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define FLAGS_NAME "e:f:ivcnhosl"

typedef struct {
  int flag_e, flag_f, flag_i, flag_v, flag_c, flag_l, flag_n, flag_h, flag_s,
      flag_o;
  int count_flag;
  int count_file;
  char *filename;
  char *pattern;
} Flags_and_Pattern;

void line_n(char *lineptr, int count_printed_line);
void add_patterns(char **patterns, char *str);
void add_file_patterns(char **patterns, char *str);
void flag_parser(Flags_and_Pattern *arg, char opt, char **patterns);
void processing_flag_c(Flags_and_Pattern arg, FILE *file, regex_t *regex);
void processing_flag_v(Flags_and_Pattern arg, FILE *file, regex_t *regex);
void processing_flag_l(Flags_and_Pattern arg, FILE *file, regex_t *regex);
void processing_flag_o(Flags_and_Pattern arg, FILE *file, regex_t *regex);
void processing_without_flag(Flags_and_Pattern arg, FILE *file, regex_t *regex);
void output(Flags_and_Pattern arg, FILE *file, regex_t *regex);

#endif