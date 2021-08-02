/*
 * flow_store.hpp
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
#ifndef __GCAP_FLOW_STORE_H__
#define __GCAP_FLOW_STORE_H__

#include "common.hpp"
#include "flow/ip4_tcp_flow.hpp"
#include "ip_flow_key.hpp"
#include "logger.hpp"
#include <map>
#include <memory>

namespace gcap {

class FlowStore {
  public:
    /**
     * Constructor
     */
    FlowStore() {}

    /**
     * Get ipv4 tcp flow
     */
    Ip4TcpFlowPtr GetIp4TcpFlow(const IpFlowKey &key);

    /**
     * Get map object
     */
    const std::map<IpFlowKey, Ip4TcpFlowPtr> &GetIp4TcpMap();

  private:
    /**
     * TCP flow map
     */
    std::map<IpFlowKey, Ip4TcpFlowPtr> tcp_flow_map_;
};

} // namespace gcap

#endif
