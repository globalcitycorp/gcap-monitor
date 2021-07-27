/*
 * processor.cpp
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

#include "processor.hpp"
#include "../util.h"
#include "ndpi_api.h"
#include <pcap.h>

namespace gcap {

Processor::Processor() {
    ndpi_module_ = ndpi_init_detection_module(ndpi_enable_ja3_plus);
    if (ndpi_module_ == NULL) {
        log_error("ndpi_init_detection_module() error!");
    }
    pcap_handle_ = NULL;
}

Processor::~Processor() {
    if (ndpi_module_) {
        ndpi_exit_detection_module(ndpi_module_);
    }
    if (pcap_handle_) {
        pcap_close(pcap_handle_);
    }
}

Processor *Processor::openPcapFile(const char *pcap_file) {
    Processor *p = new Processor();
    if (p->ndpi_module_ == NULL) {
        return NULL;
    }
    pcap_t *pcap_handle;
    char pcap_error_buffer[PCAP_ERRBUF_SIZE];
    pcap_handle = pcap_open_offline(pcap_file, pcap_error_buffer);
    if (pcap_handle == NULL) {
        log_error("pcap_open_offline() error: %s", pcap_error_buffer);
        delete p;
        return NULL;
    }
    log_debug("reading packets from pcap file %s...", pcap_file);
    p->pcap_handle_ = pcap_handle;
    return p;
}
} // namespace gcap
