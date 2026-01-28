#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int flag_b, flag_e, flag_n, flag_s, flag_t, flag_v;
} Flags;

Flags flag_parser(int argc, char **argv);
void output(Flags *arg, int argc, char *argv[]);
void processing_v_flag(unsigned char c);

#endif