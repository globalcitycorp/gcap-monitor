/*
 * flow.cpp
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

#ifndef __GCAP_FLOW_DATA_H__
#define __GCAP_FLOW_DATA_H__

#include "ndpi_api.h"
#include <pthread.h>

namespace gcap {

/**
 * Flow Data
 */
class FlowData {
  public:
    /**
     * Destructor
     */
    ~FlowData() { pthread_mutex_destroy(&mutex_); }

    /**
     * Lock flow
     */
    void Lock() { pthread_mutex_lock(&mutex_); }

    /**
     * Unlock flow
     */
    void Unlock() { pthread_mutex_unlock(&mutex_); }

  private:
    /**
     * Constructor
     */
    FlowData() {
        vlan_id_ = 0;
        l4_proto_ = 0;
        src_ip_ = 0;
        dst_ip_ = 0;
        src_port_ = 0;
        dst_port_ = 0;
        src2dst_bytes_ = 0;
        dst2src_bytes_ = 0;
        mutex_ = PTHREAD_MUTEX_INITIALIZER;
    }

    /**
     * VLAN id
     */
    u_int16_t vlan_id_;

    /**
     * L4 protocol
     */
    u_int8_t l4_proto_;

    /**
     * Source IP address
     */
    u_int32_t src_ip_;

    /**
     * Destination IP address
     */
    u_int32_t dst_ip_;

    /**
     * Source port
     */
    u_int16_t src_port_;

    /**
     * Destination port
     */
    u_int16_t dst_port_;

    /**
     * Transfered bytes from source to destination
     */
    u_int64_t src2dst_bytes_;

    /**
     * Transfered bytes from destination to source
     */
    u_int64_t dst2src_bytes_;

    /**
     * nDPI flow struct
     */
    struct ndpi_frow_struct *ndpi_flow_;

    /**
     * Source
     */
    struct ndpi_id_struct *src_;

    /**
     * Destination
     */
    struct ndpi_id_struct *dst_;

    /**
     * Mutex
     */
    pthread_mutex_t mutex_;
};

} // namespace gcap

#endif
