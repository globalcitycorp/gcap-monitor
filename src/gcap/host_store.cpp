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

#include "host_store.hpp"
#include "ndpi_api.h"
#include <unordered_map>

namespace gcap {

HostPtr HostStore::GetIp4Host(const uint32_t &ip) {
    auto itr = ip4_host_map_.find(ip);
    if (itr != ip4_host_map_.end()) {
        return itr->second;
    }
    HostPtr ptr((ndpi_id_struct *)calloc(1, SIZEOF_ID_STRUCT));
    if (ptr.get() == NULL) {
        return ptr;
    }
    auto ret = ip4_host_map_.insert(std::make_pair(ip, ptr));
    if (ret.second == false) {
        return HostPtr(NULL);
    }
    return ptr;
}

} // namespace gcap
