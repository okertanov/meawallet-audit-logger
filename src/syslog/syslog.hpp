#ifndef _AL_SYSLOG_SYSLOG_HPP_
#define _AL_SYSLOG_SYSLOG_HPP_

#pragma once

#include <string>
#include <memory>
#include <cstdarg>

#include "syslog/base/base-logger.hpp"

namespace al::syslog
{
    class syslog
    {
        public:
            static void set_inner_logger(const std::shared_ptr<al::syslog::base::base_logger> logger) {
                _logger = logger;
            }

            static std::shared_ptr<al::syslog::syslog> create(const std::string category) {
                return std::make_shared<al::syslog::syslog>(category);
            }

            template <typename T>
            static std::shared_ptr<al::syslog::syslog> create() {
                const auto category = typeid(T).name();
                return create(category);
            }

        public:
            syslog(const std::string category): _category(category) {
            }

            virtual ~syslog() {
            }

            virtual void debug(const std::string& msg, ...) {
                va_list args;
                va_start(args, msg);

                _logger->debug(_category, msg, args);

                va_end(args);
            }

            virtual void info(const std::string& msg, ...) {
                va_list args;
                va_start(args, msg);

                _logger->info(_category, msg, args);

                va_end(args);
            }

            virtual void warning(const std::string& msg, ...) {
                va_list args;
                va_start(args, msg);

                _logger->warning(_category, msg, args);

                va_end(args);
            }

            virtual void error(const std::string& msg, ...) {
                va_list args;
                va_start(args, msg);

                _logger->error(_category, msg, args);

                va_end(args);
            }

            virtual void error(const std::string& msg, const std::exception& e, ...) {
                va_list args;
                va_start(args, e);

                _logger->error(_category, msg, e, args);

                va_end(args);
            }

        private:
            const std::string _category;

            static std::shared_ptr<al::syslog::base::base_logger> _logger;
    };
}

#endif /* _AL_SYSLOG_SYSLOG_HPP_ */