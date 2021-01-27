//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_SYSLOG_BASE_BASE_LOGGER_HPP_
#define _AL_SYSLOG_BASE_BASE_LOGGER_HPP_

#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace al::syslog::base {
    class base_logger {
        public:
            base_logger() {
            }

            virtual ~base_logger() {
            }

        protected:
            std::string timestamp_str() const {
                const auto now_clock = std::chrono::system_clock::now();
                auto msecs = std::chrono::duration_cast<std::chrono::milliseconds>(now_clock.time_since_epoch()) % 1000;
                const auto now_time = std::chrono::system_clock::to_time_t(now_clock);
                auto gmt_time = gmtime(&now_time);
                auto timestamp = std::put_time(gmt_time, "%d.%m.%Y:%H:%M:%S");
                
                std::stringstream ss;
                ss << timestamp;
                ss << '.' << std::setfill('0') << std::setw(3) << msecs.count();

                return ss.str();
            }

        public:
            virtual void debug(const std::string& category, const std::string& msg) const = 0;
            virtual void info(const std::string& category, const std::string& msg) const = 0;
            virtual void warning(const std::string& category, const std::string& msg) const = 0;
            virtual void error(const std::string& category, const std::string& msg) const = 0;
            virtual void error(const std::string& category, const std::string& msg, const std::exception& e) const = 0;
    };
}

#endif /* _AL_SYSLOG_BASE_BASE_LOGGER_HPP_ */