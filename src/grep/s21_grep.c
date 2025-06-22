#include "s21_grep.h"

const char *short_options = ":e:ivcln";
const struct option long_options[] = {{NULL, 0, NULL, 0}};

int main(int argc, char *argv[]) {
  const char *utility_name = "s21_grep";
  options opt = {0};

  return process_files(argc, argv, short_options, long_options, &opt,
                       grep_option, utility_name);
  ;
}

void grep_option(FILE *file, options *opt) {
  if (!opt->pattern)
    ;
  else {
    regex_t regex;
    int flags = REG_NEWLINE | (opt->i ? REG_ICASE : 0);
    if (regcomp(&regex, opt->pattern, flags) != 0)
      ;
    else {
      char file_buffer[1024];
      int number_lines = 1, match_count = 0, has_match = 0;

      while (fgets(file_buffer, sizeof(file_buffer), file)) {
        int matched = (regexec(&regex, file_buffer, 0, NULL, 0) == 0) ^ opt->v;
        if (matched) {
          match_count++;
          if (opt->l) {
            has_match = 1;
            break;
          }
        }
        number_lines++;
      }

      if (opt->l) {
        if (has_match) printf("%s\n", opt->current_filename);
      } else if (opt->c) {
        printf("%d\n", match_count);
      } else {
        rewind(file);
        number_lines = 1;
        while (fgets(file_buffer, sizeof(file_buffer), file)) {
          int matched =
              (regexec(&regex, file_buffer, 0, NULL, 0) == 0) ^ opt->v;
          if (matched) {
            if (opt->n) printf("%d:", number_lines);
            printf("%s", file_buffer);
            if (file_buffer[strlen(file_buffer) - 1] != '\n') printf("\n");
          }
          number_lines++;
        }
      }
      regfree(&regex);
    }
  }
}