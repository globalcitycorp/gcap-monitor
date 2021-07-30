/*
 * ip4_tcp_flow.hpp
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
#ifndef __IP4_TCP_FLOW_H__
#define __IP4_TCP_FLOW_H__

#include "base_flow.hpp"
#include <IPv4Layer.h>
#include <Packet.h>
#include <TcpLayer.h>

namespace gcap {

/**
 * IPv4 TCP flow data classes.
 */
class Ip4TcpFlow : public BaseFlow {
  public:
    /**
     * Constructor
     */
    Ip4TcpFlow(u_int16_t vlan_id, u_int32_t src_ip, u_int32_t dst_ip,
               u_int64_t src_port, u_int64_t dst_port)
        : BaseFlow(), src_ip_(src_ip), dst_ip_(dst_ip), src_port_(src_port),
          dst_port_(dst_port) {}

    bool ProcessPacket(ndpi_detection_module_struct *ndpi_module,
                       const pcpp::Packet &pkt, pcpp::IPv4Layer *ipv4_layer,
                       pcpp::TcpLayer *tcp_layer, struct ndpi_id_struct *src,
                       struct ndpi_id_struct *dst, bool is_src2dst);

    inline u_int32_t GetSrcIp() const { return src_ip_; }

    inline u_int32_t GetDstIp() const { return dst_ip_; }

    inline u_int16_t GetSrcPort() const { return src_port_; }

    inline u_int16_t GetDstPort() const { return dst_port_; }

    inline bool IsSrc2DstDirection(u_int32_t src_ip, u_int16_t src_port,
                                   u_int32_t dst_ip, u_int16_t dst_port) {
        return (src_ip == src_ip_ && src_port == src_port_ &&
                dst_ip == dst_ip_ && dst_port == dst_port_);
    }

  private:
    /**
     * Source IP address
     */
    u_int32_t src_ip_;

    /**
     * Destination IP address
     */
    u_int32_t dst_ip_;

    /**
     * Source port
     */
    u_int16_t src_port_;

    /**
     * Destination port
     */
    u_int16_t dst_port_;
};

} // namespace gcap

#endif
