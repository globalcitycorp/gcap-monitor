/*
 * ip4_tcp_flow.hpp
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
#ifndef __IP4_TCP_FLOW_H__
#define __IP4_TCP_FLOW_H__

#include "../host_store.hpp"
#include "base_flow.hpp"
#include <IPv4Layer.h>
#include <Packet.h>
#include <TcpLayer.h>
#include <string>

namespace gcap {

/**
 * IPv4 TCP flow data classes.
 */
class Ip4TcpFlow : public BaseFlow {
  public:
    /**
     * Constructor
     */
    Ip4TcpFlow(u_int16_t vlan_id, u_int32_t src_ip, u_int32_t dst_ip,
               u_int64_t src_port, u_int64_t dst_port)
        : BaseFlow(), src_ip_(src_ip), dst_ip_(dst_ip), src_port_(src_port),
          dst_port_(dst_port) {
        void *ndpi_flow;
        if ((ndpi_flow = calloc(1, SIZEOF_FLOW_STRUCT)) != NULL) {
            ndpi_flow_ = (ndpi_flow_struct *)ndpi_flow;
        }
    }

    /**
     * Process packet
     */
    bool ProcessPacket(ndpi_detection_module_struct *ndpi_module,
                       const pcpp::Packet &pkt, pcpp::IPv4Layer *ipv4_layer,
                       pcpp::TcpLayer *tcp_layer, HostPtr src, HostPtr dst,
                       bool is_src2dst);

    inline u_int32_t GetSrcIp() const { return src_ip_; }

    inline u_int32_t GetDstIp() const { return dst_ip_; }

    inline u_int16_t GetSrcPort() const { return src_port_; }

    inline u_int16_t GetDstPort() const { return dst_port_; }

    inline bool IsSrc2DstDirection(u_int32_t src_ip, u_int16_t src_port,
                                   u_int32_t dst_ip, u_int16_t dst_port) {
        return (src_ip == src_ip_ && src_port == src_port_ &&
                dst_ip == dst_ip_ && dst_port == dst_port_);
    }

    /**
     * Finalize flow detection
     */
    bool Finalize(ndpi_detection_module_struct *ndpi_module) override;

  private:
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
     * Host server name
     */
    std::string host_server_name_;

    /**
     * HTTP data
     */
    struct {
        std::string last_url;
        ndpi_http_method last_method;
        std::string last_content_type;
        u_int16_t last_return_code;
    } http;

    /**
     * DNS data
     */
    struct {
        std::string last_query;
        std::string last_query_shadow;
        u_int16_t last_query_type;
        u_int16_t last_return_code;
        bool invalid_chars_in_query;
    } dns;

    /**
     * MDNS
     */
    struct {
        std::string name;
        std::string name_txt;
        std::string ssid;
        std::string answer;
    } mdns;

    /**
     * SSDP
     */
    struct {
        std::string location;
    } ssdp;

    /**
     * BitTorrent
     */
    struct {
        std::string hash;
    } bittorrent;

    /**
     * NETBIOS
     */
    struct {
        std::string name;
    } netbios;

    /**
     * SSH
     */
    struct {
        std::string client_signature;
        std::string server_signature;
        /**
         * HASSH
         * https://engineering.salesforce.com/open-sourcing-hassh-abed3ae5044c
         */
        struct {
            std::string client_hash;
            std::string server_hash;
        } hassh;
    } ssh;

    /**
     * TLS
     */
    struct {
        u_int16_t tls_version;
        u_int32_t not_before;
        u_int32_t not_after;
        std::string client_alpn;
        std::string client_tls_supported_versions;
        std::string issuer;
        std::string subject;
        std::string client_requested_server_name;
        std::string server_names;
        /**
         * JA3 hash
         * https://engineering.salesforce.com/tls-fingerprinting-with-ja3-and-ja3s-247362855967
         */
        struct {
            std::string client_hash;
            std::string server_hash;
            u_int16_t server_cipher;
            ndpi_cipher_weakness server_unsafe_cipher;
        } ja3;
    } tls;

    /**
     * Collect data from DNS packet.
     */
    void CollectDnsData();

    /**
     * Collect data from them detected by nDPI
     */
    void ProcessProtocolData();
};

using Ip4TcpFlowPtr = std::shared_ptr<Ip4TcpFlow>;

} // namespace gcap

#endif
