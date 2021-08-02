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

namespace gcap {

BaseFlow::BaseFlow()
    : first_pkt_ts_({0, 0}), last_pkt_ts_({0, 0}), src2dst_pkt_count_(0),
      dst2src_pkt_count_(0), src2dst_bytes_(0), dst2src_bytes_(0), vlan_id_(0),
      detection_completed_(false), extra_dissection_completed_(false),
      protocol_was_guessed_(false), detected_protocol_(NDPI_PROTOCOL_NULL) {
    memset(&ndpi_flow_, 0, SIZEOF_FLOW_STRUCT);
}

BaseFlow::~BaseFlow() {
    if (ndpi_flow_) {
        ndpi_free_flow(ndpi_flow_);
    }
}

bool BaseFlow::NeedsExtraDissection(ndpi_detection_module_struct *ndpi_module) {
    if (!ndpi_flow_) {
        return false;
    }
    return !extra_dissection_completed_ &&
           ndpi_extra_dissection_possible(ndpi_module, ndpi_flow_);
}

void BaseFlow::UpdateDetectedProtocol(ndpi_protocol proto) {
    if (detected_protocol_.master_protocol == NDPI_PROTOCOL_UNKNOWN) {
        detected_protocol_.master_protocol = proto.master_protocol;
    }

    if (detected_protocol_.app_protocol == NDPI_PROTOCOL_UNKNOWN ||
        ((detected_protocol_.master_protocol ==
          detected_protocol_.app_protocol) &&
         (detected_protocol_.app_protocol != proto.app_protocol))) {
        detected_protocol_.app_protocol = proto.app_protocol;
    }

    if (detected_protocol_.category == NDPI_PROTOCOL_CATEGORY_UNSPECIFIED) {
        detected_protocol_.category = proto.category;
    }
}

void BaseFlow::SetNdpiNames(ndpi_detection_module_struct *ndpi_module) {
    category_name_ =
        ndpi_category_get_name(ndpi_module, detected_protocol_.category);
    master_protocol_name_ =
        ndpi_get_proto_name(ndpi_module, detected_protocol_.master_protocol);
    app_protocol_name_ =
        ndpi_get_proto_name(ndpi_module, detected_protocol_.app_protocol);
}

} // namespace gcap
