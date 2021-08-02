/*
 * pcap_base_processor.hpp
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

#ifndef __GCAP_PCAP_BASE_PROCESSOR_H__
#define __GCAP_PCAP_BASE_PROCESSOR_H__

#include "flow_store.hpp"
#include "host_store.hpp"
#include "logger.hpp"
#include "ndpi_api.h"
#include <PcapFileDevice.h>

namespace gcap {

/**
 * Pcap file processor
 */
class PcapBaseProcessor {
  public:
    /**
     * Destructor
     */
    virtual ~PcapBaseProcessor();

    /**
     * Process packets
     *
     * @param raw_pkt packet
     * @return 0 if process ended successfully.
     */
    int ProcessPacket(pcpp::RawPacket *raw_pkt);

  protected:
    /**
     * Constructor
     */
    PcapBaseProcessor();

    /**
     * nDPI detection module
     */
    struct ndpi_detection_module_struct *ndpi_module_;

    /**
     * Flow store
     */
    FlowStore flow_store_;

    /**
     * Host store
     */
    HostStore host_store_;
};

} // namespace gcap

#endif
