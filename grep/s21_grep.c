#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Использование: grep <pattern> <file>\n");
    return 1;
  }

  Flags_and_Pattern arg = {0};
  char *patterns = NULL;
  char opt;
  while ((opt = getopt(argc, argv, FLAGS_NAME)) != -1) {
    flag_parser(&arg, opt, &patterns);
  }

  if (patterns == NULL) {
    add_patterns(&patterns, argv[optind++]);
  }

  regex_t regex;
  if (!(arg.flag_i ? regcomp(&regex, patterns, REG_ICASE | REG_EXTENDED)
                   : regcomp(&regex, patterns, REG_EXTENDED))) {
    arg.count_file = argc - optind;
    for (; optind < argc; optind++) {
      arg.filename = argv[optind];
      FILE *file = fopen(arg.filename, "r");
      if (file != NULL) {
        output(arg, file, &regex);
        fclose(file);
      } else if (!arg.flag_s) {
        fprintf(stderr, "Ошибка открытия файла\n");
      }
    }
  } else {
    fprintf(stderr, "fail regcomp");
  }
  regfree(&regex);
  free(patterns);
  return 0;
}

void flag_parser(Flags_and_Pattern *arg, char opt, char **patterns) {
  switch (opt) {
    case 'e':
      arg->flag_e = 1;
      add_patterns(patterns, optarg);
      break;
    case 'f':
      arg->flag_f = 1;
      add_file_patterns(patterns, optarg);
      break;
    case 'i':
      arg->flag_i |= REG_ICASE;
      break;
    case 'v':
      arg->flag_v = 1;
      arg->count_flag++;
      break;
    case 'c':
      arg->flag_c = 1;
      arg->count_flag++;
      break;
    case 'l':
      arg->flag_l = 1;
      arg->count_flag++;
      break;
    case 'n':
      arg->flag_n = 1;
      break;
    case 'h':
      arg->flag_h = 1;
      break;
    case 's':
      arg->flag_s = 1;
      break;
    case 'o':
      arg->flag_o = 1;
      arg->count_flag++;
      break;
  }
}

void line_n(char *lineptr, int count_printed_line) {
  char last_char_str = 'q';
  char *new_line = lineptr;
  while (*new_line != '\0') {
    last_char_str = *new_line;
    new_line++;
  }
  if (last_char_str != '\n' && count_printed_line > 0) {
    printf("\n");
  }
}

void add_patterns(char **patterns, char *str) {
  if (*patterns == NULL) {
    *patterns = calloc(1, (strlen(str) + 1));
    strcat(*patterns, str);
  } else {
    if (strlen(*patterns) > 0) {
      *patterns = realloc(
          *patterns, (sizeof(char) * (strlen(*patterns) + strlen(str) + 3)));
      strcat(*patterns, "|");
      strcat(*patterns, str);
    }
  }
}

void add_file_patterns(char **patterns, char *str) {
  char *line = NULL;
  size_t len = 0;
  FILE *file = fopen(str, "r");
  if (file != NULL) {
    while (getline(&line, &len, file) != -1) {
      size_t newline_pos = strcspn(line, "\n");
      line[newline_pos] = '\0';
      add_patterns(patterns, line);
    }
    free(line);
    fclose(file);
  } else {
    fprintf(stderr, "Ошибка открытия файла\n");
  }
}

void output(Flags_and_Pattern arg, FILE *file, regex_t *regex) {
  if (arg.flag_l)
    processing_flag_l(arg, file, regex);
  else if (arg.flag_v && !arg.flag_o && !arg.flag_c)
    processing_flag_v(arg, file, regex);
  else if (arg.flag_o && !arg.flag_v && !arg.flag_c)
    processing_flag_o(arg, file, regex);
  else {
    if (arg.flag_c) processing_flag_c(arg, file, regex);
    if ((arg.count_flag == 0 || arg.flag_i || arg.flag_h || arg.flag_n) &&
        (!arg.flag_o && !arg.flag_v))
      processing_without_flag(arg, file, regex);
  }
}

void processing_flag_l(Flags_and_Pattern arg, FILE *file, regex_t *regex) {
  char *line = NULL;
  size_t len = 0;
  regmatch_t match;
  int con = 0, flag = 0;

  while (getline(&line, &len, file) != -1 && flag != 1) {
    if (arg.flag_v ? !regexec(regex, line, 1, &match, 0) == 0
                   : regexec(regex, line, 1, &match, 0) == 0) {
      ++con;
      if (con == 1) {
        printf("%s\n", arg.filename);
        flag = 1;
      }
    }
  }
  free(line);
}

void processing_flag_v(Flags_and_Pattern arg, FILE *file, regex_t *regex) {
  char *line = NULL;
  size_t len = 0;
  int count_string = 0;
  int number_string = 0;

  while (getline(&line, &len, file) != -1) {
    ++number_string;
    if (regexec(regex, line, 0, 0, 0) == 1) {
      if (arg.count_file == 1 || arg.flag_h) {
        if (regexec(regex, line, 0, 0, 0)) {
          ++count_string;
          arg.flag_n ? printf("%d:%s", number_string, line)
                     : printf("%s", line);
        }
      } else {
        arg.flag_n ? printf("%s:%d:%s", arg.filename, number_string, line)
                   : printf("%s:%s", arg.filename, line);
        ++count_string;
      }
      line_n(line, count_string);
    }
  }
  free(line);
}

void processing_flag_o(Flags_and_Pattern arg, FILE *file, regex_t *regex) {
  char *lineptr_o = NULL;
  size_t len_o = 0;
  int count_str = 1;
  while (getline(&lineptr_o, &len_o, file) != -1) {
    regmatch_t pmatch[1];
    regoff_t len;
    char *line = lineptr_o;
    while (!regexec(regex, line, 1, pmatch, 0)) {
      len = pmatch[0].rm_eo - pmatch[0].rm_so;
      if (arg.count_file == 1 || arg.flag_h) {
        arg.flag_n
            ? printf("%d:%.*s\n", count_str, (int)len, line + pmatch[0].rm_so)
            : printf("%.*s\n", (int)len, line + pmatch[0].rm_so);
      } else {
        arg.flag_n ? printf("%s:%d:%.*s\n", arg.filename, count_str, (int)len,
                            line + pmatch[0].rm_so)
                   : printf("%s:%.*s\n", arg.filename, (int)len,
                            line + pmatch[0].rm_so);
      }
      line += pmatch[0].rm_eo;
    }
    count_str++;
  }
  free(lineptr_o);
}

void processing_flag_c(Flags_and_Pattern arg, FILE *file, regex_t *regex) {
  char *line = NULL;
  size_t len = 0;
  int count_string = 0;

  while (getline(&line, &len, file) != -1) {
    size_t newline_pos = strcspn(line, "\n");
    line[newline_pos] = '\0';
    if (arg.flag_v ? regexec(regex, line, 0, 0, 0)
                   : regexec(regex, line, 0, 0, 0) == 0) {
      count_string++;
    }
  }
  if (arg.count_file == 1 || arg.flag_h) {
    printf("%d\n", count_string);
  } else
    printf("%s:%d\n", arg.filename, count_string);
  free(line);
}

void processing_without_flag(Flags_and_Pattern arg, FILE *file,
                             regex_t *regex) {
  char *line = NULL;
  size_t len = 0;
  regmatch_t match;
  int number_string = 0;
  int count_string = 0;

  while (getline(&line, &len, file) != -1) {
    ++number_string;
    if (regexec(regex, line, 1, &match, 0) == 0) {
      ++count_string;
      if (arg.count_file == 1 || arg.flag_h) {
        arg.flag_n ? printf("%d:%s", number_string, line) : printf("%s", line);
      } else {
        arg.flag_n ? printf("%s:%d:%s", arg.filename, number_string, line)
                   : printf("%s:%s", arg.filename, line);
      }
      line_n(line, count_string);
    }
  }
  free(line);
}
