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

namespace gcap {

PcapFileProcessor::PcapFileProcessor() : PcapBaseProcessor() { reader_ = NULL; }

PcapFileProcessor::~PcapFileProcessor() {
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
        p->logger_.Err() << "unable to open pcap file!" << std::endl;
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
        std::cout << "First pkt time: " << FormatTimespec(flow->GetFirstPktTs())
                  << std::endl
                  << "Last pkt time:  " << FormatTimespec(flow->GetLastPktTs())
                  << std::endl
                  << pcpp::IPv4Address(flow->GetSrcIp()).toString() << ":"
                  << flow->GetSrcPort() << " -> "
                  << pcpp::IPv4Address(flow->GetDstIp()).toString() << ":"
                  << flow->GetDstPort() << std::endl
                  << "Category: " << flow->GetCategoryName(ndpi_module_)
                  << "; Master protocol: "
                  << flow->GetMasterProtocolName(ndpi_module_)
                  << "; App protocol: "
                  << flow->GetAppProtocolName(ndpi_module_) << std::endl
                  << "src2dst pkts: " << flow->GetSrc2DstPktCount()
                  << ", bytes: " << flow->GetSrc2DstBytes() << std::endl
                  << "dst2src pkts: " << flow->GetDst2SrcPktCount()
                  << ", bytes: " << flow->GetDst2SrcBytes() << std::endl
                  << std::endl;
    }
    return 0;
}

} // namespace gcap
