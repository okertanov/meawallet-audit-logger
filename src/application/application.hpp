//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_APPLICATION_APPLICATION_HPP_
#define _AL_APPLICATION_APPLICATION_HPP_

#pragma once

#include <memory>
#include <string>
#include <exception>
#include <stdexcept>

#include "syslog/syslog.hpp"
#include "config/config.hpp"
#include "crypto/crypto.hpp"
#include "audit/base/base-logger.hpp"
#include "audit/file/file-logger.hpp"
#include "interceptor/base-interceptor.hpp"
#include "interceptor/crypto-interceptor.hpp"
#include "storage/base-storage.hpp"
#include "storage/secure-storage.hpp"

namespace al::application {
    class application {
        public:
            application(): _logger(al::syslog::syslog::create("application")) {
            }

            virtual ~application() {
                terminate();
            }

            virtual const application* bootstrap() const {
                // Config is supposed to be assigned above this scope.
                // Assign the rest dependencies, to be re-assigned in testing environment for example.
                // To be convergted to Dependency Injection/Inverse of control to resolve these deps dynamically.

                const auto crypto = std::make_shared<al::crypto::crypto>();
                this->with(crypto);

                const auto storage = std::make_shared<al::storage::secure_storage>(crypto);
                this->with(storage);

                const auto interceptor = std::make_shared<al::interceptor::crypto_interceptor>(storage, crypto);
                this->with(interceptor);

                const auto audit_logger = std::make_shared<al::audit::file::file_logger>(storage, interceptor);
                this->with(audit_logger);

                return this;
            }

            virtual const application* with(const std::shared_ptr<al::config::config> config) const {
                _config = config;
                return this;
            }

            virtual const application* with(const std::shared_ptr<al::crypto::crypto> crypto) const {
                _crypto = crypto;
                return this;
            }

            virtual const application* with(const std::shared_ptr<al::interceptor::base_interceptor> interceptor) const {
                _interceptor = interceptor;
                return this;
            }

            virtual const application* with(const std::shared_ptr<al::storage::base_storage> storage) const {
                _storage = storage;
                return this;
            }

            virtual const application* with(const std::shared_ptr<al::audit::base::base_logger> audit_logger) const {
                _audit_logger = audit_logger;
                return this;
            }

            virtual const application* run(void) const {
                run_impl();
                return this;
            }

            virtual void terminate(void) const {
            }

        private:
            void run_impl(void) const {
                // If help option - show usage and exit.
                if (_config->is_help()) {
                    _config->show_usage();
                    return;
                }

                // Initialize storage first, then verify its integrity.
                _storage->initialize();
                const auto storage_state = _storage->verify();

                _logger->info(std::string("Secure storage: ") +
                    (storage_state == al::storage::EMPTY ? "empty" :
                     storage_state == al::storage::GOOD ? "good" :
                     storage_state == al::storage::BAD ? "bad" :
                     storage_state == al::storage::NO_ACCESS ? "no access" :
                     "unknown")
                );

                // Select its current running mode: audit log entry producer or log dumper.
                if (_config->is_reset_mode()) {
                    _logger->debug("Active mode: reset secure storage and audit log");
                    run_reset_mode_impl();
                }
                else if(_config->is_monitor_mode()) {
                    _logger->debug("Active mode: audit log monitoring");
                    run_monitor_mode_impl();
                }
                else if (_config->is_count_mode()) {
                    _logger->debug("Active mode: audit log dump (records count only)");
                    run_count_mode_impl();
                }
                else if (_config->is_dump_mode()) {
                    _logger->debug("Active mode: audit log dump");
                    run_dump_mode_impl();
                }
                else if (_config->is_log_mode()) {
                    _logger->debug("Active mode: audit log add entry");
                    run_log_mode_impl();
                }
                else {
                    throw std::runtime_error("Wrong application mode");
                }
            }

            void run_reset_mode_impl(void) const {
                _storage->reset();
            }

            void run_monitor_mode_impl(void) const {
                // TODO: use file monitor
            }

            void run_count_mode_impl(void) const {
                const auto audit_log_records_number = _audit_logger->count_audit_log();
                std::cout << "Records #: " << audit_log_records_number << std::endl;
            }

            void run_dump_mode_impl(void) const {
                const auto records = _audit_logger->read_audit_log();
                std::cout << (records.length() > 0 ? records : "<AUDIT LOG EMPTY>\n");
            }

            void run_log_mode_impl(void) const {
                const auto data = _config->get_entry();
                _audit_logger->append_audit_log(data);
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            mutable std::shared_ptr<al::config::config> _config;
            mutable std::shared_ptr<al::crypto::crypto> _crypto;
            mutable std::shared_ptr<al::interceptor::base_interceptor> _interceptor;
            mutable std::shared_ptr<al::storage::base_storage> _storage;
            mutable std::shared_ptr<al::audit::base::base_logger> _audit_logger;
    };
}

#endif /* _AL_APPLICATION_APPLICATION_HPP_ */