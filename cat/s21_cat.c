#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
  } else {
    Flags arg = flag_parser(argc, argv);
    output(&arg, argc, argv);
  }
  return 0;
}
