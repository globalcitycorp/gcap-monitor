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
#include "logger.hpp"
#include "ndpi_api.h"
#include "util.hpp"
#include <memory>

namespace gcap {

PcapFileProcessor::PcapFileProcessor(Queue<BaseFlowPtr> *flow_queue)
    : PcapBaseProcessor(flow_queue) {
    reader_ = NULL;
}

PcapFileProcessor::~PcapFileProcessor() {
    if (reader_) {
        if (reader_->isOpened()) {
            reader_->close();
        }
        delete reader_;
    }
}

PcapFileProcessor *PcapFileProcessor::Open(const char *pcap_file,
                                           Queue<BaseFlowPtr> *flow_queue) {
    LoggerPtr logger = Logger::GetInstance();
    PcapFileProcessor *p = new PcapFileProcessor(flow_queue);
    if (p->ndpi_module_ == NULL) {
        return NULL;
    }
    pcpp::IFileReaderDevice *reader =
        pcpp::IFileReaderDevice::getReader(pcap_file);
    if (!reader->open()) {
        logger->Write(Logger::Level::ERR, "unable to open pcap file!");
        delete reader;
        return NULL;
    }
    p->reader_ = reader;
    return p;
}

int PcapFileProcessor::Process() {
    pcpp::RawPacket raw_pkt;
    while (reader_->getNextPacket(raw_pkt)) {
        ProcessPacket(&raw_pkt);
    }
    auto ip4_tcp_map = flow_store_.GetIp4TcpMap();
    for (auto itr = ip4_tcp_map.begin(); itr != ip4_tcp_map.end(); ++itr) {
        auto flow = itr->second;
        flow->Finalize(ndpi_module_);
        flow_queue_->enqueue(std::static_pointer_cast<BaseFlow>(flow));
    }
    return 0;
}

} // namespace gcap
