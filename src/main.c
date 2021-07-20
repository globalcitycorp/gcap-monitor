/*
 * main.c
 * Copyright (C) 2021-21 - Globalciy, Corp.
 *
 * This project is using nDPI.
 *
 * Copyright (C) 2011-21 - ntop.org
 *
 * nDPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nDPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with nDPI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ndpi_api.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_READER_THREADS 16

typedef struct Config {
  char *interface;
  u_int8_t num_threads;
} Config;

static void parse_options(int argc, char **argv, struct Config *cnf);

int main(int argc, char **argv) {
  Config cnf = {.interface = "\n", .num_threads = 1};

  if (ndpi_get_api_version() != NDPI_API_VERSION) {
    printf("nDPI Library version mismatch: "
           "please make sure this code and the nDPI library are in sync\n");
    return -1;
  }

  printf("Using nDPI (%s).\n", ndpi_revision());

  parse_options(argc, argv, &cnf);

  printf("Listening interface %s with %d thread(s).\n", cnf.interface,
         cnf.num_threads);

  return 0;
}

static struct option long_opts[] = {
    {"interface", required_argument, NULL, 'i'},
    {"num-threads", required_argument, NULL, 'n'},

    {0, 0, 0, 0}};

/**
 * Parse options
 */
void parse_options(int argc, char **argv, struct Config *cnf) {
  int option_idx = 0;
  int opt;
  int num_threads;
  while ((opt = getopt_long(argc, argv, "i:n:", long_opts, &option_idx)) !=
         EOF) {
    switch (opt) {
    case 'i':
      cnf->interface = optarg;
      break;
    case 'n':
      num_threads = atoi(optarg);
      if (num_threads > MAX_NUM_READER_THREADS) {
        num_threads = MAX_NUM_READER_THREADS;
      }
      cnf->num_threads = num_threads;
    }
  }
  return;
}
