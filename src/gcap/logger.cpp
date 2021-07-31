/*
 * logger.cpp
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

#include "logger.hpp"
#include <iomanip>
#include <iostream>

std::ostream &gcap::Logger::Err() {
    PrintTimestamp(std::cerr);
    return std::cerr << "[ERROR] ";
}

std::ostream &gcap::Logger::Dbg() {
    PrintTimestamp(std::cout);
    return std::cout << "[DEBUG] ";
}

void gcap::Logger::PrintTimestamp(std::ostream &s) {
    time_t t = time(NULL);
    const tm *localt = localtime(&t);
    int y = localt->tm_year + 1900;
    int m = localt->tm_mon + 1;
    int d = localt->tm_mday;
    int hr = localt->tm_hour;
    int min = localt->tm_min;
    int sec = localt->tm_sec;
    s << y << (m < 10 ? "-0" : "-") << m << (d < 10 ? "-0" : "-") << d;
    s << (hr < 10 ? " 0" : " ") << hr << (min < 10 ? ":0" : ":") << min
      << (sec < 10 ? ":0" : ":") << sec;
    s << " ";
};
