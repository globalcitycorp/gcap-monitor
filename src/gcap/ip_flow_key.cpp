/*
 * ip_flow_key.cpp
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

#include "ip_flow_key.hpp"
#include "ndpi_api.h"

/**
 * Operator <
 */
bool gcap::operator<(const gcap::IpFlowKey l, const gcap::IpFlowKey r) {
    if (l.hash_ != r.hash_) {
        return l.hash_ < r.hash_;
    }
    if (l.vlan_id_ != r.vlan_id_) {
        return l.vlan_id_ < r.vlan_id_;
    }
    bool same = (l.src_ip_ == r.src_ip_ && l.src_port_ == r.src_port_ &&
                 l.dst_ip_ == r.dst_ip_ && l.dst_port_ == r.dst_port_);
    bool opposite = (l.src_ip_ == r.dst_ip_ && l.src_port_ == r.dst_port_ &&
                     l.dst_ip_ == r.src_ip_ && l.dst_port_ == r.src_port_);
    if (same || opposite) {
        return false;
    }
    if (l.src_ip_ != r.src_ip_) {
        return l.src_ip_ < r.src_ip_;
    }
    if (l.src_port_ != r.src_port_) {
        return l.src_port_ < r.src_port_;
    }
    if (l.dst_ip_ != r.dst_ip_) {
        return l.dst_ip_ < r.dst_ip_;
    }
    if (l.dst_port_ != r.dst_port_) {
        return l.dst_port_ < r.dst_port_;
    }
    return false;
}
