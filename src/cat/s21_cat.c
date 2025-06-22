#include "s21_cat.h"

const char *short_options = "beEnstTv";

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {NULL, 0, NULL, 0}};

int main(int argc, char *argv[]) {
  const char *utility_name = "s21_cat";
  options opt = {0};

  return process_files(argc, argv, short_options, long_options, &opt,
                       cat_option, utility_name);
}

void cat_option(FILE *file, options *opt) {
  int ch;
  int prev_ch = '\n';
  int number_lines = 1;
  int consecutive_blank_lines = 0;

  while ((ch = getc(file)) != EOF) {
    if (opt->s && ch == '\n' && prev_ch == '\n') {
      consecutive_blank_lines++;
    } else {
      consecutive_blank_lines = 0;
    }

    if (!(opt->s && consecutive_blank_lines > 1)) {
      if (prev_ch == '\n') {
        if (opt->b && ch != '\n') {
          printf("%6d\t", number_lines++);
        } else if (opt->n && !opt->b) {
          printf("%6d\t", number_lines++);
        }
      }

      char_option_cat((unsigned char)ch, opt);
    }

    prev_ch = ch;
  }
}

void char_option_cat(unsigned char ch, options *opt) {
  opt->print_ch_done = 0;

  if (ch == '\t' && opt->t) {
    printf("^I");
    opt->print_ch_done = 1;
  }

  if (!opt->print_ch_done && ch == '\n') {
    if (opt->e) {
      printf("$");
    }
    putchar('\n');
    opt->print_ch_done = 1;
  }

  if (!opt->print_ch_done && opt->v) {
    if (ch < 32 && ch != '\n' && ch != '\t') {
      printf("^%c", ch + 64);
      opt->print_ch_done = 1;
    } else if (ch == 127) {
      printf("^?");
      opt->print_ch_done = 1;
    } else if (ch > 127) {
      printf("M-%c", ch & 0x7F);
      opt->print_ch_done = 1;
    } else if (ch == '\0') {
      printf("^@");
      opt->print_ch_done = 1;
    }
  }

  if (!opt->print_ch_done) {
    putchar(ch);
  }
}