/*
 * stdout_writer.cpp
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

#include "./stdout_writer.hpp"
#include "../common.hpp"
#include "../flow/flows.hpp"
#include "../util.hpp"
#include <iostream>

namespace gcap {

TypeResult StdoutWriter::WriteOut(Ip4TcpFlowPtr flow) {
    std::cout << "First pkt time: " << FormatTimespec(flow->GetFirstPktTs())
              << std::endl
              << "Last pkt time:  " << FormatTimespec(flow->GetLastPktTs())
              << std::endl
              << pcpp::IPv4Address(flow->GetSrcIp()).toString() << ":"
              << flow->GetSrcPort() << " -> "
              << pcpp::IPv4Address(flow->GetDstIp()).toString() << ":"
              << flow->GetDstPort()
              << std::endl
              // << "Category: " << flow->GetCategoryName(ndpi_module_)
              << "; Master protocol: "
              // << flow->GetMasterProtocolName(ndpi_module_)
              // << "; App protocol: " << flow->GetAppProtocolName(ndpi_module_)
              << std::endl
              << "src2dst pkts: " << flow->GetSrc2DstPktCount()
              << ", bytes: " << flow->GetSrc2DstBytes() << std::endl
              << "dst2src pkts: " << flow->GetDst2SrcPktCount()
              << ", bytes: " << flow->GetDst2SrcBytes() << std::endl
              << std::endl;
}

} // namespace gcap
