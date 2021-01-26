#ifndef _AL_SYSLOG_CONSOLE_CONSOLE_LOGGER_HPP_
#define _AL_SYSLOG_CONSOLE_CONSOLE_LOGGER_HPP_

#pragma once

#include <string>
#include <iostream>

#include "syslog/base/base-logger.hpp"

namespace al::syslog::console
{
    class console_logger: public al::syslog::base::base_logger
    {
        public:
            virtual void debug(const std::string& category, const std::string& msg, ...) {
                std::cout << timestamp_str() << ": " << category << ": " << msg;
                std::cout << std::endl;
            }

            virtual void info(const std::string& category, const std::string& msg, ...) {
                std::cout << timestamp_str() << ": " << category << ": " << msg;
                std::cout << std::endl;
            }

            virtual void warning(const std::string& category, const std::string& msg, ...) {
                std::cout << timestamp_str() << ": " << category << ": " << msg;
                std::cout << std::endl;
            }

            virtual void error(const std::string& category, const std::string& msg, ...) {
                std::cerr << timestamp_str() << ": " << category << ": " << msg;
                std::cerr << std::endl;
            }

            virtual void error(const std::string& category, const std::string& msg, const std::exception& e, ...) {
                std::cerr << timestamp_str() << ": " << category << ": " << msg << e.what();
                std::cerr << std::endl;
            }
    };
}

#endif /* _AL_SYSLOG_CONSOLE_CONSOLE_LOGGER_HPP_ */