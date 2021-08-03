/*
 * pcap_file_processor.hpp
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

#ifndef __GCAP_PCAP_FILE_PROCESSOR_H__
#define __GCAP_PCAP_FILE_PROCESSOR_H__

#include "flow/base_flow.hpp"
#include "flow_store.hpp"
#include "host_store.hpp"
#include "logger.hpp"
#include "ndpi_api.h"
#include "pcap_base_processor.hpp"
#include "queue.hpp"
#include <PcapFileDevice.h>

namespace gcap {

/**
 * Pcap file processor
 */
class PcapFileProcessor : public PcapBaseProcessor {
  public:
    /**
     * Open pcap file and return processor.
     */
    static PcapFileProcessor *Open(const char *pcap_file,
                                   Queue<BaseFlowPtr> *flow_queue);

    /**
     * Process packets
     *
     * @return 0 if process ended successfully.
     */
    int Process();

    /**
     * Destructor
     */
    ~PcapFileProcessor();

  private:
    /**
     * Constructor is private
     */
    PcapFileProcessor(Queue<BaseFlowPtr> *flow_queue);

    /**
     * pcap file reader
     */
    pcpp::IFileReaderDevice *reader_;
};

} // namespace gcap

#endif
