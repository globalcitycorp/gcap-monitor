/*
 * main.cpp
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

#include "gcap/logger.hpp"
#include "gcap/pcap_file_processor.hpp"
#include "ndpi_api.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_READER_THREADS 16

typedef struct gcap_config {
    const char *interface;
    const char *pcap_file;
    u_int8_t num_threads;
} gcap_config_t;

static void parse_options(int argc, char **argv, gcap_config_t *cnf);

int main(int argc, char **argv) {
    gcap_config_t cnf = {.interface = "", .pcap_file = "", .num_threads = 1};
    gcap::Logger logger;

    if (ndpi_get_api_version() != NDPI_API_VERSION) {
        printf("nDPI Library version mismatch: "
               "please make sure this code and the nDPI library are in sync\n");
        return -1;
    }

    printf("Using nDPI (%s).\n", ndpi_revision());

    parse_options(argc, argv, &cnf);
    if (strcmp(cnf.interface, "") == 0 && strcmp(cnf.pcap_file, "") == 0) {
        printf("Please specify interface or pcap file!\n");
        return -1;
    }

    if (strcmp(cnf.pcap_file, "") != 0) {
        logger.Dbg() << "Reading pcap file " << cnf.pcap_file << "..."
                     << std::endl;
        gcap::PcapFileProcessor *processor =
            gcap::PcapFileProcessor::Open(cnf.pcap_file);
        if (processor == NULL) {
            logger.Err() << "Unable to open " << cnf.pcap_file << "."
                         << std::endl;
            return 1;
        }
        processor->Process();
        delete processor;
    }

    return 0;
}

static struct option long_opts[] = {
    {"interface", required_argument, NULL, 'i'},
    {"pcapfile", required_argument, NULL, 'p'},
    {"num-threads", required_argument, NULL, 'n'},

    {0, 0, 0, 0}};

/**
 * Parse options
 */
void parse_options(int argc, char **argv, gcap_config_t *cnf) {
    int option_idx = 0;
    int opt;
    int num_threads;
    while ((opt = getopt_long(argc, argv, "i:p:n:", long_opts, &option_idx)) !=
           EOF) {
        switch (opt) {
        case 'i':
            cnf->interface = optarg;
            break;
        case 'p':
            cnf->pcap_file = optarg;
            break;
        case 'n':
            num_threads = atoi(optarg);
            if (num_threads > MAX_NUM_READER_THREADS) {
                num_threads = MAX_NUM_READER_THREADS;
            }
            cnf->num_threads = num_threads;
            break;
        }
    }
    return;
}
