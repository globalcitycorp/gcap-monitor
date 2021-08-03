/*
 * stdout_writer.hpp
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
#ifndef __GCAP_STDOUT_WRITER_H__
#define __GCAP_STDOUT_WRITER_H__

#include "../flow/flows.hpp"
#include "base_writer.hpp"

namespace gcap {

/**
 * Base writer class to output flow.
 */
class StdoutWriter : public BaseWriter {
  public:
    /**
     * Constructor
     */
    StdoutWriter() : BaseWriter() {}

    /**
     * Destructor
     */
    virtual ~StdoutWriter() {}

    /**
     * Write out IPv4 TCP flow.
     * @param flow Flow
     * @return result
     */
    WriteResult WriteOutIp4TcpFlow(Ip4TcpFlowPtr flow);
};

} // namespace gcap

#endif
