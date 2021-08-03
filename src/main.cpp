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

#include "gcap/flow/base_flow.hpp"
#include "gcap/logger.hpp"
#include "gcap/pcap_file_processor.hpp"
#include "gcap/queue.hpp"
#include "gcap/writer/base_writer.hpp"
#include "gcap/writer/stdout_writer.hpp"
#include "ndpi_api.h"
#include <csignal>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>

#define MAX_NUM_READER_THREADS 16
#define FLOW_QUEUE_SIZE 1024

typedef struct GcapConfigStruct {
    const char *interface;
    const char *pcap_file;
    u_int8_t num_threads;
} GcapConfig;

static struct option long_opts[] = {
    {"interface", required_argument, NULL, 'i'},
    {"pcapfile", required_argument, NULL, 'p'},
    {"num-threads", required_argument, NULL, 'n'},

    {0, 0, 0, 0}};

/**
 * Parse options
 */
void ParseOptions(int argc, char **argv, GcapConfig *cnf) {
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

/**
 * Process pcap file.
 */
static void ProcessPcapFile(const char *pcap_file,
                            gcap::Queue<gcap::BaseFlowPtr> *flow_queue);

/**
 * Writer out flow.
 */
static void OutputFlows(gcap::BaseWriter *writer,
                        gcap::Queue<gcap::BaseFlowPtr> *flow_queue);

int main(int argc, char **argv) {
    GcapConfig cnf = {.interface = "", .pcap_file = "", .num_threads = 1};
    gcap::Logger::Initialize();
    gcap::LoggerPtr logger = gcap::Logger::GetInstance();

    if (ndpi_get_api_version() != NDPI_API_VERSION) {
        printf("nDPI Library version mismatch: "
               "please make sure this code and the nDPI library are in sync\n");
        return -1;
    }

    printf("Using nDPI (%s).\n", ndpi_revision());

    ParseOptions(argc, argv, &cnf);
    if (strcmp(cnf.interface, "") == 0 && strcmp(cnf.pcap_file, "") == 0) {
        printf("Please specify interface or pcap file!\n");
        return -1;
    }

    gcap::Queue<gcap::BaseFlowPtr> flow_queue(FLOW_QUEUE_SIZE);

    gcap::StdoutWriter *writer = new gcap::StdoutWriter();

    std::thread th_write(OutputFlows, writer, &flow_queue);

    if (strcmp(cnf.pcap_file, "") != 0) {
        logger->Write(gcap::Logger::Level::DEBUG,
                      std::string("Reading pcap file ") + cnf.pcap_file +
                          "...");
        std::thread th_proc_file(ProcessPcapFile, cnf.pcap_file, &flow_queue);
        th_proc_file.join();
    }

    th_write.join();

    delete writer;

    return 0;
}

void ProcessPcapFile(const char *pcap_file,
                     gcap::Queue<gcap::BaseFlowPtr> *flow_queue) {
    gcap::LoggerPtr logger = gcap::Logger::GetInstance();
    gcap::PcapFileProcessor *processor =
        gcap::PcapFileProcessor::Open(pcap_file, flow_queue);
    if (processor == NULL) {
        logger->Write(gcap::Logger::Level::ERR,
                      std::string("Unable to open ") + pcap_file + ".");
        return;
    }
    processor->Process();
    delete processor;
}

void OutputFlows(gcap::BaseWriter *writer,
                 gcap::Queue<gcap::BaseFlowPtr> *flow_queue) {
    while (true) {
        gcap::BaseFlowPtr flow = flow_queue->dequeue();
        if (flow.get() == NULL) {
            break;
        }
        writer->WriteOut(flow);
    }
}
