#ifndef S21_COMMON_H
#define S21_COMMON_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_BUFFER_MAX 4096

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int print_ch_done;
  char *pattern;
  const char *current_filename;
  int i;
  int c;
  int l;
} options;

int scan_flags(int argc, char **argv, options *opt, const char *short_opts,
               const struct option *long_opts, const char *utility_name);
int parse_flag(int option, options *opt, int is_cat, int is_grep);
void handle_file_error(const char *utility_name, const char *filename);
int process_files(int argc, char *argv[], const char *short_options,
                  const struct option *long_options, options *opt,
                  void (*file_processor)(FILE *, options *),
                  const char *utility_name);
#endif