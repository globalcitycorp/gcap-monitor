/*
 * base_writer.cpp
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

#include "base_writer.hpp"
#include "../logger.hpp"

namespace gcap {

WriteResult BaseWriter::WriteOut(BaseFlowPtr flow) {
    Ip4TcpFlowPtr ipv4_tcp_flow = std::dynamic_pointer_cast<Ip4TcpFlow>(flow);
    if (ipv4_tcp_flow.get() != NULL) {
        return WriteOutIp4TcpFlow(ipv4_tcp_flow);
    }
    return true;
};

} // namespace gcap
