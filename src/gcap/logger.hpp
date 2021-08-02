/*
 * logger.hpp
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
#ifndef __GCAP_LOGGER_H__
#define __GCAP_LOGGER_H__

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <syslog.h>

namespace gcap {

using TargetType = int64_t;

/**
 * Logger
 *
 */
class Logger {
  public:
    enum class Target : TargetType {
        DISABLED = 1,
        STDOUT = 2,
        STDERR = 4,
        LOG_FILE = 8
    };

    enum class Level : int16_t {
        DEBUG = LOG_DEBUG,
        INFO = LOG_INFO,
        NOTICE = LOG_NOTICE,
        WARNING = LOG_WARNING,
        ERR = LOG_ERR,
        CRIT = LOG_CRIT,
        ALERT = LOG_ALERT,
        EMERG = LOG_EMERG
    };

    /**
     * Get logger instance
     */
    static std::shared_ptr<Logger> GetInstance();

    /**
     * Initialize a singleton instance
     */
    static void Initialize();

    /**
     * Write out message
     */
    void Write(Level level, std::string msg);

  private:
    /**
     * Constructor
     */
    Logger() { target_ = Target::STDOUT; }

    /**
     * Target
     */
    Target target_;

    std::map<Level, std::string> levelMap = {
        {Level::DEBUG, "DEBUG"},   {Level::INFO, "INFO"},
        {Level::NOTICE, "NOTICE"}, {Level::WARNING, "WARNING"},
        {Level::ERR, "ERROR"},     {Level::CRIT, "CRITICAL"},
        {Level::ALERT, "ALERT"},   {Level::EMERG, "EMERG"}};
};

using LoggerPtr = std::shared_ptr<Logger>;

inline Logger::Target operator&(Logger::Target a, Logger::Target b) {
    return static_cast<Logger::Target>(static_cast<TargetType>(a) &
                                       static_cast<TargetType>(b));
}

inline Logger::Target operator|(Logger::Target a, Logger::Target b) {
    return static_cast<Logger::Target>(static_cast<TargetType>(a) |
                                       static_cast<TargetType>(b));
}

inline bool operator!(Logger::Target a) {
    return a == static_cast<Logger::Target>(0);
}

} // namespace gcap

#endif
