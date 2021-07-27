/*
 * flow_key.cpp
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
#ifndef __GCAP_FLOW_KEY_H__
#define __GCAP_FLOW_KEY_H__

#include "ndpi_api.h"

namespace gcap {

/**
 * Flow key
 */
class FlowKey {
  public:
    /**
     * Constructor
     */
    FlowKey(u_int16_t vlan_id, u_int8_t l4_proto, u_int32_t src_ip,
            u_int16_t src_port, u_int32_t dst_ip, u_int16_t dst_port)
        : vlan_id_(vlan_id), l4_proto_(l4_proto), src_ip_(src_ip),
          src_port_(src_port), dst_ip_(dst_ip), dst_port_(dst_port) {
        hash_ = vlan_id + l4_proto + src_ip + dst_ip + src_port + dst_port;
    }

    /**
     * Comparer
     *
     * This is for using the instance as key in std::map.
     */
    bool operator<(const FlowKey &r);

  private:
    /**
     * VLAN id
     */
    u_int16_t vlan_id_;

    /**
     * L4 protocol
     */
    u_int8_t l4_proto_;

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

    /**
     * Hash to compare
     */
    u_int32_t hash_;
};

/**
 * Operator <
 */
bool FlowKey::operator<(const FlowKey &r) {
    if (hash_ != r.hash_) {
        return hash_ < r.hash_;
    }
    if (vlan_id_ != r.vlan_id_) {
        return vlan_id_ < r.vlan_id_;
    }
    if (l4_proto_ != r.l4_proto_) {
        return l4_proto_ < r.l4_proto_;
    }
    bool same = (src_ip_ == r.src_ip_ && src_port_ == r.src_port_ &&
                 dst_ip_ == r.dst_ip_ && dst_port_ == r.dst_port_);
    bool opposite = (src_ip_ == r.dst_ip_ && src_port_ == r.dst_port_ &&
                     dst_ip_ == r.src_ip_ && dst_port_ == r.src_port_);
    if (same || opposite) {
        return false;
    }
    if (src_ip_ != r.src_ip_) {
        return src_ip_ < r.src_ip_;
    }
    if (src_port_ != r.src_port_) {
        return src_port_ < r.src_port_;
    }
    if (dst_ip_ != r.dst_ip_) {
        return dst_ip_ < r.dst_ip_;
    }
    if (dst_port_ != r.dst_port_) {
        return dst_port_ < r.dst_port_;
    }
    return false;
}

} // namespace gcap

#endif
