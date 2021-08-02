/*
 * pcap_base_processor.cpp
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

#include "pcap_base_processor.hpp"
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

PcapBaseProcessor::PcapBaseProcessor() {
    LoggerPtr logger = Logger::GetInstance();
    ndpi_module_ = ndpi_init_detection_module(ndpi_enable_ja3_plus);
    if (ndpi_module_ == NULL) {
        logger->Write(Logger::Level::ERR,
                      "ndpi_init_detection_module() error!");
    }
    NDPI_PROTOCOL_BITMASK all;
    NDPI_BITMASK_SET_ALL(all);
    ndpi_set_protocol_detection_bitmask2(ndpi_module_, &all);
    ndpi_finalize_initialization(ndpi_module_);
}

PcapBaseProcessor::~PcapBaseProcessor() {
    if (ndpi_module_) {
        ndpi_exit_detection_module(ndpi_module_);
    }
}

int PcapBaseProcessor::ProcessPacket(pcpp::RawPacket *raw_pkt_ptr) {
    LoggerPtr logger = Logger::GetInstance();
    pcpp::Packet pkt(raw_pkt_ptr);
    pcpp::EthLayer *eth_layer = pkt.getLayerOfType<pcpp::EthLayer>();
    uint16_t vlan_id = 0;
    if (eth_layer == NULL) {
        logger->Write(Logger::Level::ERR, "unable to find ethernet layer");
        return 1;
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
                logger->Write(Logger::Level::ERR, "unable to allocate flow");
                return 1;
            }
            HostPtr src = host_store_.GetIp4Host(flow->GetSrcIp());
            HostPtr dst = host_store_.GetIp4Host(flow->GetDstIp());
            if (src == NULL || dst == NULL) {
                logger->Write(Logger::Level::ERR,
                              "unable to allocate src or dst id struct");
                return 1;
            }
            bool is_src2dst =
                flow->IsSrc2DstDirection(src_ip, src_port, dst_ip, dst_port);
            flow->ProcessPacket(ndpi_module_, pkt, ipv4_layer, tcp_layer, src,
                                dst, is_src2dst);
        }
        return 0;
    }
    return 1;
}

} // namespace gcap
