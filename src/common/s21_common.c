#include "s21_common.h"

int scan_flags(int argc, char **argv, options *opt, const char *short_opts,
               const struct option *long_opts, const char *utility_name) {
  int success = 1;
  opterr = 0;

  int is_cat = strcmp(utility_name, "s21_cat") == 0;
  int is_grep = strcmp(utility_name, "s21_grep") == 0;

  int option;
  while ((option = getopt_long(argc, argv, short_opts, long_opts, NULL)) !=
         -1) {
    if (!parse_flag(option, opt, is_cat, is_grep)) {
      success = 0;
    }
  }

  if (is_grep && !opt->pattern && optind < argc) {
    opt->pattern = argv[optind];
    optind++;
  }

  if (is_cat && opt->b) {
    opt->n = 0;
  }

  return success;
}

int parse_flag(int option, options *opt, int is_cat, int is_grep) {
  int success = 1;

  if (option == 'b' && is_cat) {
    opt->b = 1;
  } else if (option == 'e') {
    if (is_grep) {
      if (optarg) {
        opt->pattern = optarg;
      } else {
        success = 0;
      }
    } else if (is_cat) {
      opt->e = 1;
      opt->v = 1;
    }
  } else if (option == 'E' && is_cat) {
    opt->e = 1;
  } else if (option == 'n') {
    opt->n = 1;
  } else if (option == 's' && is_cat) {
    opt->s = 1;
  } else if (option == 't' && is_cat) {
    opt->t = 1;
    opt->v = 1;
  } else if (option == 'T' && is_cat) {
    opt->t = 1;
  } else if (option == 'v') {
    opt->v = 1;
  } else if (option == 'i' && is_grep) {
    opt->i = 1;
  } else if (option == 'c' && is_grep) {
    opt->c = 1;
  } else if (option == 'l' && is_grep) {
    opt->l = 1;
  } else {
    success = 0;
  }

  return success;
}

int process_files(int argc, char *argv[], const char *short_options,
                  const struct option *long_options, options *opt,
                  void (*file_processor)(FILE *, options *),
                  const char *utility_name) {
  int status = 0;

  int needs_pattern = (strcmp(utility_name, "s21_grep") == 0);

  if (!scan_flags(argc, argv, opt, short_options, long_options, utility_name)) {
    fprintf(stderr, "Error\n");
    status = 1;
  } else {
    if (needs_pattern) {
      if (!opt->pattern && optind < argc) {
        opt->pattern = argv[optind++];
      }

      if (!opt->pattern) {
        fprintf(stderr, "%s: missing pattern\n", utility_name);
        status = 1;
      }
    }

    if (status == 0) {
      int file_counter = optind;
      while (file_counter < argc) {
        FILE *file = fopen(argv[file_counter], "r");

        if (!file) {
          handle_file_error(utility_name, argv[file_counter]);
        } else {
          opt->current_filename = argv[file_counter];
          file_processor(file, opt);
          fclose(file);
        }
        file_counter++;
      }
    }
  }

  return status;
}

void handle_file_error(const char *utility_name, const char *filename) {
  fprintf(stderr, "%s: %s: No such file or directory\n", utility_name,
          filename);
}
