/*
 * flow_store.cpp
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

#include "flow_store.hpp"
#include "flow/ip4_tcp_flow.hpp"
#include "ip_flow_key.hpp"
#include "logger.hpp"
#include <IpAddress.h>
#include <map>
#include <memory>

namespace gcap {

Ip4TcpFlowPtr FlowStore::GetIp4TcpFlow(const IpFlowKey &key) {
    if (tcp_flow_map_.count(key) == 1) {
        return tcp_flow_map_.at(key);
    }
    Ip4TcpFlowPtr ptr(new Ip4TcpFlow(key.vlan_id_, key.src_ip_, key.dst_ip_,
                                     key.src_port_, key.dst_port_));
    if (ptr.get() == NULL) {
        return ptr;
    }
    auto ret = tcp_flow_map_.insert(std::make_pair(key, ptr));
    if (ret.second == false) {
        return Ip4TcpFlowPtr(NULL);
    }
    return ptr;
}

const std::map<IpFlowKey, Ip4TcpFlowPtr> &FlowStore::GetIp4TcpMap() {
    return tcp_flow_map_;
}

} // namespace gcap
