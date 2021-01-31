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
                const auto record = _storage->read_secure_storage_record();
                
                if (record.records_count != count) {
                    throw std::runtime_error("Data corrupted: audit log lines number mismatch");
                }

                return count;
            }

            virtual const std::string read_audit_log(void) const {
                // TODO: Fix to not to read all data at once, mmap/asio/iterators etc.
                const auto lines = _storage->read_audit_log();
                const auto record = _storage->read_secure_storage_record();

                if (record.records_count != lines.size()) {
                    throw std::runtime_error("Data corrupted: audit log lines number mismatch");
                }

                std::stringstream lites_stringstream;

                for (const auto& line : lines) {
                    const auto line_decoded = _interceptor->transform(line);
                    lites_stringstream << line_decoded << std::endl;
                }

                return lites_stringstream.str();
            }

            virtual void append_audit_log(const std::string& line) const {
                const auto line_transformed = _interceptor->transform(line);
                _storage->append_audit_log(line);

                auto record = _storage->read_secure_storage_record();
                record.records_count += 1;
                _storage->write_secure_storage_record(record);
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            const std::shared_ptr<al::storage::base_storage> _storage;
            const std::shared_ptr<al::interceptor::base_interceptor> _interceptor;
    };
}

#endif /* _AL_AUDIT_FILE_FILE_LOGGER_HPP_ */