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
#include "flow_store.hpp"
#include "logger.hpp"
#include "ndpi_api.h"
#include <EthLayer.h>
#include <IPv4Layer.h>
#include <IPv6Layer.h>
#include <Packet.h>
#include <TcpLayer.h>
#include <UdpLayer.h>
#include <VlanLayer.h>

namespace gcap {

PcapFileProcessor::PcapFileProcessor() {
    ndpi_module_ = ndpi_init_detection_module(ndpi_enable_ja3_plus);
    if (ndpi_module_ == NULL) {
        logger_.Err() << "ndpi_init_detection_module() error!" << std::endl;
    }
    NDPI_PROTOCOL_BITMASK all;
    NDPI_BITMASK_SET_ALL(all);
    ndpi_set_protocol_detection_bitmask2(ndpi_module_, &all);
    ndpi_finalize_initialization(ndpi_module_);
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
        pcpp::Packet pkt(&raw_pkt);
        pcpp::EthLayer *eth_layer = pkt.getLayerOfType<pcpp::EthLayer>();
        uint16_t vlan_id = 0;
        if (eth_layer == NULL) {
            logger_.Err() << "unable to find ethernet layer" << std::endl;
            continue;
        }
        pcpp::VlanLayer *vlan_layer = pkt.getLayerOfType<pcpp::VlanLayer>();
        if (vlan_layer != NULL) {
            vlan_id = vlan_layer->getVlanID();
        }
        pcpp::IPv4Layer *ipv4_layer = pkt.getLayerOfType<pcpp::IPv4Layer>();
        if (ipv4_layer != NULL) {
            pcpp::TcpLayer *tcp_layer = pkt.getLayerOfType<pcpp::TcpLayer>();
            if (tcp_layer != NULL) {
                uint32_t src_ip = ipv4_layer->getSrcIPv4Address().toInt();
                uint32_t dst_ip = ipv4_layer->getDstIPv4Address().toInt();
                uint16_t src_port = tcp_layer->getSrcPort();
                uint16_t dst_port = tcp_layer->getDstPort();
                IpFlowKey key(vlan_id, src_ip, src_port, dst_ip, dst_port);
                Ip4TcpFlowPtr flow = flow_store_.GetIp4TcpFlow(key);
                if (flow == NULL) {
                    logger_.Err() << "unable to allocate flow" << std::endl;
                    continue;
                }
                HostPtr src = host_store_.GetIp4Host(flow->GetSrcIp());
                HostPtr dst = host_store_.GetIp4Host(flow->GetDstIp());
                if (src == NULL || dst == NULL) {
                    logger_.Err() << "unable to allocate src or dst id struct"
                                  << std::endl;
                    continue;
                }
                bool is_src2dst = flow->IsSrc2DstDirection(src_ip, src_port,
                                                           dst_ip, dst_port);
                flow->ProcessPacket(ndpi_module_, pkt, ipv4_layer, tcp_layer,
                                    src, dst, is_src2dst);
            }
        }
    }
    auto ip4_tcp_map = flow_store_.GetIp4TcpMap();
    for (auto itr = ip4_tcp_map.begin(); itr != ip4_tcp_map.end(); ++itr) {
        auto flow = itr->second;
        std::cout << pcpp::IPv4Address(flow->GetSrcIp()).toString() << ":"
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
