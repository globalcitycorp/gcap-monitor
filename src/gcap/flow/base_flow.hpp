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
#ifndef __GCAP_FLOW_KEY_H__
#define __GCAP_FLOW_KEY_H__

#include <Packet.h>

namespace gcap {

/**
 * Base class of flow data classes.
 */
class BaseFlow {
  public:
    /**
     * Constructor
     */
    BaseFlow();

  private:
    /**
     * Timestamp of the first packet.
     */
    timespec first_pkt_ts;

    /**
     * Timestamp of the last packet ever received.
     */
    timespec last_pkt_ts;

    /**
     * Packets count of src to dst direction.
     */
    uint16_t src2dst_pkt_count;

    /**
     * Packets count of dst to src direction.
     */
    uint16_t dst2src_pkt_count;
};

} // namespace gcap

#endif
