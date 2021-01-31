//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_AUDIT_FILE_FILE_LOGGER_HPP_
#define _AL_AUDIT_FILE_FILE_LOGGER_HPP_

#pragma once

#include <memory>

#include "syslog/syslog.hpp"
#include "audit/base/base-logger.hpp"
#include "storage/base-storage.hpp"
#include "interceptor/base-interceptor.hpp"

namespace al::audit::file {
    class file_logger: public al::audit::base::base_logger {
        public:
            file_logger(
                std::shared_ptr<al::storage::base_storage> storage,
                std::shared_ptr<al::interceptor::base_interceptor> interceptor) :
                _logger(al::syslog::syslog::create("file_logger")),
                _storage(storage),
                _interceptor(interceptor) {
            }

            virtual ~file_logger() {
            }

            virtual unsigned long long count_audit_log(void) const {
                const auto count = _storage->count_audit_log();
                return count;
            }

            virtual const std::string read_audit_log(void) const {
                // TODO: Fix to not to read all data at once, mmap/asio/iterators etc.
                const auto lines = _storage->read_audit_log();
                const auto lines_decoded = _interceptor->transform(lines);
                return lines;
            }

            virtual void append_audit_log(const std::string& line) const {
                const auto line_transformed = _interceptor->transform(line);
                _storage->append_audit_log(line);
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            const std::shared_ptr<al::storage::base_storage> _storage;
            const std::shared_ptr<al::interceptor::base_interceptor> _interceptor;
    };
}

#endif /* _AL_AUDIT_FILE_FILE_LOGGER_HPP_ */