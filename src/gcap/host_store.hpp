/*
 * host_store.cpp
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
    inline ndpi_id_struct *GetIp4Host(const uint32_t &ip);

  private:
    /**
     * IPv4 host map
     */
    std::unordered_map<uint32_t, std::unique_ptr<ndpi_id_struct>> ip4_host_map_;
};

ndpi_id_struct *HostStore::GetIp4Host(const uint32_t &ip) {
    auto itr = ip4_host_map_.find(ip);
    if (itr != ip4_host_map_.end()) {
        return itr->second.get();
    }
    ndpi_id_struct *raw_ptr = (ndpi_id_struct *)calloc(1, SIZEOF_ID_STRUCT);
    if (raw_ptr == NULL) {
        return NULL;
    }
    std::unique_ptr<ndpi_id_struct> ptr(raw_ptr);
    auto ret = ip4_host_map_.insert(make_pair(ip, ptr));
    if (ret.second == false) {
        return NULL;
    }
    return ptr.get();
}

} // namespace gcap

#endif
