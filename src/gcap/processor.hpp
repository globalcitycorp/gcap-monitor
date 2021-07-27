/*
 * processor.hpp
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

#include "ndpi_api.h"
#include <pcap.h>

namespace gcap {

/**
 * Pcap processor
 */
class Processor {
  public:
    /**
     * Open pcap file and return processor.
     */
    static Processor *OpenPcapFile(const char *pcap_file);

    /**
     * Open and listen network device and return processor.
     */
    static Processor *OpenDevice(const char *device);

    /**
     * Destructor
     */
    ~Processor();

  private:
    /**
     * Constructor is private
     */
    Processor();

    /**
     * pcap handle
     */
    pcap_t *pcap_handle_;

    /**
     * nDPI detection module
     */
    struct ndpi_detection_module_struct *ndpi_module_;
};

} // namespace gcap
