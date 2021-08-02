/*
 * ip4_tcp_flow.cpp
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
#include "ip4_tcp_flow.hpp"
#include <IPv4Layer.h>
#include <Packet.h>
#include <cstring>
#include <iostream>

namespace gcap {

bool Ip4TcpFlow::ProcessPacket(ndpi_detection_module_struct *ndpi_module,
                               const pcpp::Packet &pkt,
                               pcpp::IPv4Layer *ipv4_layer,
                               pcpp::TcpLayer *tcp_layer, HostPtr src,
                               HostPtr dst, bool is_src2dst) {
    ProcessPacketCommon(pkt, is_src2dst);

    if (detection_completed_ && extra_dissection_completed_) {
        return true;
    }

    if (!ndpi_flow_) {
        return;
    }

    timespec ts = pkt.getRawPacket()->getPacketTimeStamp();
    u_int64_t ts_ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

    ndpi_protocol proto = ndpi_detection_process_packet(
        ndpi_module, ndpi_flow_, ipv4_layer->getDataPtr(),
        ipv4_layer->getDataLen(), ts_ms, src.get(), dst.get());

    u_int8_t detected = ndpi_is_protocol_detected(ndpi_module, proto);

    if (detected) {
        UpdateDetectedProtocol(proto);
        detection_completed_ = true;
    }

    if (detection_completed_ && extra_dissection_completed_) {
        return true;
    }

    u_int16_t l7proto = ndpi_get_lower_proto(proto);

    switch (l7proto) {
    case NDPI_PROTOCOL_DNS:
        CollectDnsData();
        break;
    }

    if (detection_completed_ && (!NeedsExtraDissection(ndpi_module))) {
        // Update category by IP
        ndpi_fill_ip_protocol_category(ndpi_module, src_ip_, dst_ip_,
                                       GetDetectedProtocolPtr());
        ProcessProtocolData();
        extra_dissection_completed_ = true;
        UpdateDetectedProtocol(proto);
    }

    return true;
}

void Ip4TcpFlow::CollectDnsData() {
    if (!ndpi_flow_) {
        return;
    }

    ndpi_flow_->check_extra_packets = 1;
    ndpi_flow_->max_extra_packets_to_check = 10;
    if (ndpi_flow_->host_server_name[0] != '\0') {
        if (ndpi_flow_->protos.dns.is_query) {
            std::string query((char *)ndpi_flow_->host_server_name);
            if (!query.empty()) {
                bool invalid_found = false;
                for (int i = 0; query[i] != '\0'; i++) {
                    if (!isprint(query[i])) {
                        query[i] = '?';
                        invalid_found = true;
                    }
                }
                dns.invalid_chars_in_query = invalid_found;
                dns.last_query_shadow = dns.last_query;
                dns.last_query = query;
                dns.last_query_type = ndpi_flow_->protos.dns.query_type;
            }
        }
    }
}

void Ip4TcpFlow::ProcessProtocolData() {
    if (!ndpi_flow_) {
        return;
    }

    ndpi_protocol proto = GetDetectedProtocol();
    u_int16_t l7proto = ndpi_get_lower_proto(proto);

    /* host_server_name in DHCP is for the
       client name, not the server */
    if (l7proto != NDPI_PROTOCOL_DNS && l7proto != NDPI_PROTOCOL_DHCP &&
        ndpi_flow_->host_server_name[0] != '\0' && host_server_name_.empty()) {

        if (ndpi_is_proto(proto, NDPI_PROTOCOL_HTTP)) {
            char *colon = strrchr((char *)ndpi_flow_->host_server_name, ':');
            if (colon) {
                colon[0] = '\0';
            }
        }
        host_server_name_ = std::string((char *)ndpi_flow_->host_server_name);
    }

    switch (l7proto) {
    case NDPI_PROTOCOL_SSH:
        if (ssh.client_signature.empty()) {
            ssh.client_signature =
                std::string(ndpi_flow_->protos.ssh.client_signature);
        }
        if (ssh.server_signature.empty()) {
            ssh.server_signature =
                std::string(ndpi_flow_->protos.ssh.server_signature);
        }
        if (ssh.hassh.client_hash.empty() &&
            ndpi_flow_->protos.ssh.hassh_client[0] != '\0') {
            ssh.hassh.client_hash =
                std::string(ndpi_flow_->protos.ssh.hassh_client);
        }
        if (ssh.hassh.server_hash.empty() &&
            ndpi_flow_->protos.ssh.hassh_server[0] != '\0') {
            ssh.hassh.server_hash =
                std::string(ndpi_flow_->protos.ssh.hassh_server);
        }
        break;

    case NDPI_PROTOCOL_BITTORRENT:
        if (bittorrent.hash.empty()) {
            bittorrent.hash =
                std::string((char *)ndpi_flow_->protos.bittorrent.hash);
        }
        break;

    case NDPI_PROTOCOL_MDNS:
        if (ndpi_flow_->host_server_name[0] != '\0' && mdns.answer.empty())
            mdns.answer = std::string((char *)ndpi_flow_->host_server_name);
        break;

    case NDPI_PROTOCOL_TLS:
    case NDPI_PROTOCOL_MAIL_IMAPS:
    case NDPI_PROTOCOL_MAIL_SMTPS:
    case NDPI_PROTOCOL_MAIL_POPS:
        tls.tls_version = ndpi_flow_->protos.tls_quic_stun.tls_quic.ssl_version;
        tls.not_before = ndpi_flow_->protos.tls_quic_stun.tls_quic.notBefore,
        tls.not_after = ndpi_flow_->protos.tls_quic_stun.tls_quic.notAfter;

        if (tls.client_requested_server_name.empty() &&
            (ndpi_flow_->protos.tls_quic_stun.tls_quic
                 .client_requested_server_name[0] != '\0')) {
            tls.client_requested_server_name =
                std::string(ndpi_flow_->protos.tls_quic_stun.tls_quic
                                .client_requested_server_name);
        }

        if (tls.server_names.empty() &&
            ndpi_flow_->protos.tls_quic_stun.tls_quic.server_names != NULL) {
            tls.server_names = std::string(
                ndpi_flow_->protos.tls_quic_stun.tls_quic.server_names);
        }

        if (tls.client_alpn.empty() &&
            ndpi_flow_->protos.tls_quic_stun.tls_quic.alpn != NULL) {
            tls.client_alpn =
                std::string(ndpi_flow_->protos.tls_quic_stun.tls_quic.alpn);
        }

        if (tls.client_tls_supported_versions.empty() &&
            ndpi_flow_->protos.tls_quic_stun.tls_quic.tls_supported_versions !=
                NULL) {
            tls.client_tls_supported_versions =
                std::string(ndpi_flow_->protos.tls_quic_stun.tls_quic
                                .tls_supported_versions);
        }

        if (tls.issuer.empty() &&
            ndpi_flow_->protos.tls_quic_stun.tls_quic.issuerDN != NULL) {
            tls.issuer =
                std::string(ndpi_flow_->protos.tls_quic_stun.tls_quic.issuerDN);
        }

        if (tls.subject.empty() &&
            ndpi_flow_->protos.tls_quic_stun.tls_quic.subjectDN != NULL) {
            tls.subject = std::string(
                ndpi_flow_->protos.tls_quic_stun.tls_quic.subjectDN);
        }

        if (tls.ja3.client_hash.empty() &&
            ndpi_flow_->protos.tls_quic_stun.tls_quic.ja3_client[0] != '\0') {
            tls.ja3.client_hash = std::string(
                ndpi_flow_->protos.tls_quic_stun.tls_quic.ja3_client);
        }

        if (tls.ja3.server_hash.empty() &&
            ndpi_flow_->protos.tls_quic_stun.tls_quic.ja3_server[0] != '\0') {
            tls.ja3.server_hash = std::string(
                ndpi_flow_->protos.tls_quic_stun.tls_quic.ja3_server);
            tls.ja3.server_unsafe_cipher =
                ndpi_flow_->protos.tls_quic_stun.tls_quic.server_unsafe_cipher;
            tls.ja3.server_cipher =
                ndpi_flow_->protos.tls_quic_stun.tls_quic.server_cipher;
        }
        break;

    case NDPI_PROTOCOL_HTTP:
    case NDPI_PROTOCOL_HTTP_PROXY:
        if (ndpi_flow_->http.url &&
            ndpi_flow_->http.method != NDPI_HTTP_METHOD_UNKNOWN) {
            http.last_url = std::string(ndpi_flow_->http.url);
            http.last_method = ndpi_flow_->http.method;
        }

        break;
    } /* switch */
}

bool Ip4TcpFlow::Finalize(ndpi_detection_module_struct *ndpi_module) {
    // check ndpi_flow_ is allocated
    if (!detection_completed_ && ndpi_flow_) {
        u_int8_t guessed;
        UpdateDetectedProtocol(
            ndpi_detection_giveup(ndpi_module, ndpi_flow_, 1, &guessed));
        protocol_was_guessed_ = guessed ? true : false;
        detection_completed_ = true;
    }

    if (!extra_dissection_completed_) {
        ndpi_fill_ip_protocol_category(ndpi_module, src_ip_, dst_ip_,
                                       GetDetectedProtocolPtr());
        ProcessProtocolData();
        extra_dissection_completed_ = true;
    }

    SetNdpiNames(ndpi_module);
}

} // namespace gcap
