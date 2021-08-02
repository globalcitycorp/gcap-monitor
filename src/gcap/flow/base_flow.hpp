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
#ifndef __GCAP_BASE_FLOW_H__
#define __GCAP_BASE_FLOW_H__

#include "ndpi_api.h"
#include <Packet.h>
#include <string>

namespace gcap {

/**
 * Base class of flow data classes.
 */
class BaseFlow {

  public:
    virtual ~BaseFlow() {}

    /**
     * Get nDPI category name
     */
    inline std::string GetCategoryName() { return category_name_; }

    /**
     * Get nDPI master protocol name
     */
    inline std::string GetMasterProtocolName() { return master_protocol_name_; }

    /**
     * Get nDPI app protocol name
     */
    inline std::string GetAppProtocolName() { return app_protocol_name_; }

    /**
     * Get timestamp of the first packet.
     */
    inline timespec GetFirstPktTs() { return first_pkt_ts_; }

    /**
     * Get timestamp of the last packet received.
     */
    inline timespec GetLastPktTs() { return last_pkt_ts_; }

    /**
     * Get count of packets from source to destination direction
     */
    inline uint16_t GetSrc2DstPktCount() { return src2dst_pkt_count_; }

    /**
     * Get count of packets from destination to source direction
     */
    inline uint16_t GetDst2SrcPktCount() { return dst2src_pkt_count_; }

    /**
     * Get count of all packets in the flow
     */
    inline uint16_t GetPktCount() {
        return src2dst_pkt_count_ + dst2src_pkt_count_;
    }

    /**
     * Get bytes from source to destination direction
     */
    inline uint64_t GetSrc2DstBytes() { return src2dst_bytes_; }

    /**
     * Get bytes from destination to source direction
     */
    inline uint64_t GetDst2SrcBytes() { return dst2src_bytes_; }

    /**
     * Get bytes of the flow
     */
    inline uint16_t GetBytes() { return src2dst_bytes_ + dst2src_bytes_; }

    /**
     * Finalize flow detection
     */
    virtual bool Finalize(ndpi_detection_module_struct *ndpi_module) = 0;

  protected:
    /**
     * Constructor
     */
    BaseFlow();

    /**
     * Timestamp of the first packet.
     */
    timespec first_pkt_ts_;

    /**
     * Timestamp of the last packet ever received.
     */
    timespec last_pkt_ts_;

    /**
     * Packets count of src to dst direction.
     */
    uint16_t src2dst_pkt_count_;

    /**
     * Packets count of dst to src direction.
     */
    uint16_t dst2src_pkt_count_;

    /**
     * Transfered bytes from source to destination
     */
    u_int64_t src2dst_bytes_;

    /**
     * Transfered bytes from destination to source
     */
    u_int64_t dst2src_bytes_;

    /**
     * VLAN id
     */
    u_int16_t vlan_id_;

    /**
     * nDPI flow struct
     */
    ndpi_flow_struct ndpi_flow_;

    /**
     * Whether detection completed or not
     */
    bool detection_completed_;

    /**
     * Whether extra dissection completed or not
     */
    bool extra_dissection_completed_;

    bool protocol_was_guessed_;

    /**
     * nDPI category name
     */
    std::string category_name_;

    /**
     * nDPI master protocol name
     */
    std::string master_protocol_name_;

    /**
     * nDPI app protocol name
     */
    std::string app_protocol_name_;

    /**
     * Get detected protocol
     */
    inline ndpi_protocol GetDetectedProtocol() { return detected_protocol_; }

    /**
     * Get detected protocol to give to nDPI internal process.
     */
    inline ndpi_protocol *GetDetectedProtocolPtr() {
        return &detected_protocol_;
    }

    /**
     * Update detected protocol.
     */
    void UpdateDetectedProtocol(ndpi_protocol proto);

    /**
     * Common process
     */
    inline bool ProcessPacketCommon(const pcpp::Packet &pkt, bool is_src2dst);

    /**
     * Check if flow needs extra dissection.
     */
    bool NeedsExtraDissection(ndpi_detection_module_struct *ndpi_module);

    /**
     * Set nDPI category name and protocol names.
     */
    void SetNdpiNames(ndpi_detection_module_struct *ndpi_module);

  private:
    /**
     * Detected protocol
     */
    ndpi_protocol detected_protocol_;
};

bool BaseFlow::ProcessPacketCommon(const pcpp::Packet &pkt, bool is_src2dst) {
    timespec ts = pkt.getRawPacket()->getPacketTimeStamp();
    if (first_pkt_ts_.tv_sec == 0 && first_pkt_ts_.tv_nsec == 0) {
        first_pkt_ts_ = ts;
    }
    last_pkt_ts_ = ts;
    if (is_src2dst) {
        src2dst_pkt_count_++;
        src2dst_bytes_ += pkt.getFirstLayer()->getDataLen();
    } else {
        dst2src_pkt_count_++;
        dst2src_bytes_ += pkt.getFirstLayer()->getDataLen();
    }
    return true;
}

} // namespace gcap

#endif
