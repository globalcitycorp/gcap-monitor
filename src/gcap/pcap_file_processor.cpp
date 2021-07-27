/*
 * pcap_file_processor.cpp
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

#include "pcap_file_processor.hpp"
#include "../util.h"
#include "ndpi_api.h"
#include <pcap.h>

namespace gcap {

PcapFileProcessor::PcapFileProcessor() {
    ndpi_module_ = ndpi_init_detection_module(ndpi_enable_ja3_plus);
    if (ndpi_module_ == NULL) {
        LogDebug("ndpi_init_detection_module() error!");
    }
    reader_ = NULL;
}

PcapFileProcessor::~PcapFileProcessor() {
    if (ndpi_module_) {
        ndpi_exit_detection_module(ndpi_module_);
    }
    if (reader_) {
        if (reader_->isOpened()) {
            reader_->close();
        }
        delete reader_;
    }
}

PcapFileProcessor *PcapFileProcessor::Open(const char *pcap_file) {
    PcapFileProcessor *p = new PcapFileProcessor();
    if (p->ndpi_module_ == NULL) {
        return NULL;
    }
    pcpp::IFileReaderDevice *reader =
        pcpp::IFileReaderDevice::getReader(pcap_file);
    if (!reader->open()) {
        LogDebug("unable to open pcap file!");
        delete reader;
        return NULL;
    }
    p->reader_ = reader;
    return p;
}

} // namespace gcap
