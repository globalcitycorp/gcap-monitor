/*
 * base_flow.hpp
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
#ifndef __GCAP_BASE_FLOW_H__
#define __GCAP_BASE_FLOW_H__

#include <Packet.h>

namespace gcap {

/**
 * Base class of flow data classes.
 */
class BaseFlow {

  public:
    virtual bool ProcessPacket(pcpp::Packet pkt);

  protected:
    /**
     * Constructor
     */
    BaseFlow();

    /**
     * Timestamp of the first packet.
     */
    timespec first_pkt_ts_;

    /**
     * Timestamp of the last packet ever received.
     */
    timespec last_pkt_ts_;

    /**
     * Packets count of src to dst direction.
     */
    uint16_t src2dst_pkt_count_;

    /**
     * Packets count of dst to src direction.
     */
    uint16_t dst2src_pkt_count_;

    /**
     * Transfered bytes from source to destination
     */
    u_int64_t src2dst_bytes_;

    /**
     * Transfered bytes from destination to source
     */
    u_int64_t dst2src_bytes_;

    /**
     * VLAN id
     */
    u_int16_t vlan_id_;

    /**
     * Common process
     */
    inline bool ProcessPacketCommon(pcpp::Packet pkt, bool is_src2dst);
};

bool BaseFlow::ProcessPacketCommon(pcpp::Packet pkt, bool is_src2dst) {
    timespec ts = pkt.getRawPacket()->getPacketTimeStamp();
    if (first_pkt_ts_.tv_sec == 0 && first_pkt_ts_.tv_nsec == 0) {
        first_pkt_ts_ = ts;
    }
    last_pkt_ts_ = ts;
    if (is_src2dst) {
        src2dst_pkt_count_++;
        src2dst_bytes_ += pkt.getFirstLayer()->getDataLen();
    } else {
        dst2src_pkt_count_++;
        dst2src_bytes_ += pkt.getFirstLayer()->getDataLen();
    }
}

} // namespace gcap

#endif
