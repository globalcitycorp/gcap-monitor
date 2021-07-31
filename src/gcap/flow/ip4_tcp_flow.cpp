/*
 * ip4_tcp_flow.cpp
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
#include "ip4_tcp_flow.hpp"
#include <IPv4Layer.h>
#include <Packet.h>

namespace gcap {

bool Ip4TcpFlow::ProcessPacket(ndpi_detection_module_struct *ndpi_module,
                               const pcpp::Packet &pkt,
                               pcpp::IPv4Layer *ipv4_layer,
                               pcpp::TcpLayer *tcp_layer,
                               struct ndpi_id_struct *src,
                               struct ndpi_id_struct *dst, bool is_src2dst) {
    ProcessPacketCommon(pkt, is_src2dst);
    timespec ts = pkt.getRawPacket()->getPacketTimeStamp();
    u_int64_t ts_ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    detected_protocol_ = ndpi_detection_process_packet(
        ndpi_module, &ndpi_flow_, ipv4_layer->getDataPtr(),
        ipv4_layer->getDataLen(), ts_ms, src, dst);
    return true;
}

} // namespace gcap
