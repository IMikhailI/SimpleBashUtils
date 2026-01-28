#include "s21_cat.h"

Flags flag_parser(int argc, char **argv) {
  Flags arg = {0};
  struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                  {"number", 0, NULL, 'n'},
                                  {"squeeze-blank", 0, NULL, 's'},
                                  {NULL, 0, NULL, 0}};
  int opt;
  while ((opt = getopt_long(argc, argv, "beEsntTv", long_options, 0)) != -1) {
    switch (opt) {
      case 'b':
        arg.flag_b = 1;
        break;
      case 'n':
        arg.flag_n = 1;
        break;
      case 's':
        arg.flag_s = 1;
        break;
      case 'e':
        arg.flag_e = 1;
        arg.flag_v = 1;
        break;
      case 'E':
        arg.flag_e = 1;
        break;
      case 't':
        arg.flag_t = 1;
        arg.flag_v = 1;
        break;
      case 'T':
        arg.flag_t = 1;
        break;
      case 'v':
        arg.flag_v = 1;
        break;
      default:
        fprintf(stderr, "Try 'cat --help' for more information.\n");
        exit(1);
    }
  }
  return arg;
}
