/*
 * base_flow.cpp
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
#include "base_flow.hpp"
#include "ndpi_api.h"
#include <Packet.h>
#include <iostream>

gcap::BaseFlow::BaseFlow()
    : first_pkt_ts_({0, 0}), last_pkt_ts_({0, 0}), src2dst_pkt_count_(0),
      dst2src_pkt_count_(0), src2dst_bytes_(0), dst2src_bytes_(0), vlan_id_(0),
      detected_protocol_(NDPI_PROTOCOL_NULL) {
    memset(&ndpi_flow_, 0, SIZEOF_FLOW_STRUCT);
    std::cout << "BaseFlow()" << std::endl;
}
