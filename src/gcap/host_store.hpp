/*
 * host_store.hpp
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
#ifndef __GCAP_HOST_STORE_H__
#define __GCAP_HOST_STORE_H__

#include "ndpi_api.h"
#include <memory>
#include <unordered_map>

namespace gcap {

using HostPtr = std::shared_ptr<ndpi_id_struct>;

/**
 * Stores ndpi_id_struct
 *
 */
class HostStore {
  public:
    /**
     * Constructor
     */
    HostStore() {}

    /**
     * Get ipv4 tcp flow
     */
    HostPtr GetIp4Host(const uint32_t &ip);

  private:
    /**
     * IPv4 host map
     */
    std::unordered_map<uint32_t, HostPtr> ip4_host_map_;
};

} // namespace gcap

#endif
