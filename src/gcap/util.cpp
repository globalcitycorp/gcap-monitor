/*
 * util.cpp
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

#include "util.hpp"
#include <ctime>
#include <stdlib.h>
#include <string>

namespace gcap {

std::string FormatTimespec(timespec ts) {
    char s[128];
    struct tm t;
    localtime_r((time_t *)&ts.tv_sec, &t);
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d.%09ld", t.tm_year + 1900,
            t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, ts.tv_nsec);
    return std::string(s);
}

} // namespace gcap
