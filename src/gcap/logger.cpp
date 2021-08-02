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
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

using std::chrono::system_clock;

namespace gcap {

static std::shared_ptr<Logger> log;

void Logger::Initialize() { log = std::shared_ptr<Logger>(new Logger()); }

LoggerPtr Logger::GetInstance() { return log; }

void Logger::Write(Logger::Level level, std::string msg) {
    std::time_t time = system_clock::to_time_t(system_clock::now());
    struct tm *time_struct = std::localtime(&time);
    char time_str[80];
    strftime(time_str, 80, "%Y-%m-%d:%H:%M:%S %z ", time_struct);
    std::string level_str =
        std::string("[") + levelMap[level] + std::string("] ");

    if (!!(this->target_ & Logger::Target::STDOUT)) {
        std::cout << time_str << level_str << msg << std::endl;
    }
}
} // namespace gcap
