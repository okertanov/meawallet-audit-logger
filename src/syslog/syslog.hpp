//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_SYSLOG_SYSLOG_HPP_
#define _AL_SYSLOG_SYSLOG_HPP_

#pragma once

#include <memory>
#include <string>

#include "syslog/base/base-logger.hpp"

namespace al::syslog {
    class syslog {
        public:
            static void set_inner_logger(const std::shared_ptr<al::syslog::base::base_logger> logger) {
                _logger = logger;
            }

            static std::shared_ptr<al::syslog::syslog> create(const std::string category) {
                return std::make_shared<al::syslog::syslog>(category);
            }

        public:
            syslog(const std::string category): _category(category) {
            }

            virtual ~syslog() {
            }

            virtual void debug(const std::string& msg) const {
                _logger->debug(_category, msg);
            }

            virtual void info(const std::string& msg) const {
                _logger->info(_category, msg);
            }

            virtual void warning(const std::string& msg) const {
                _logger->warning(_category, msg);
            }

            virtual void error(const std::string& msg) const {
                _logger->error(_category, msg);
            }

            virtual void error(const std::string& msg, const std::exception& e) const {
                _logger->error(_category, msg, e);
            }

        private:
            const std::string _category;

            static std::shared_ptr<al::syslog::base::base_logger> _logger;
    };
}

#endif /* _AL_SYSLOG_SYSLOG_HPP_ */