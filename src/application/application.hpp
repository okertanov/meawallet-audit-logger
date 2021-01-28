//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_APPLICATION_APPLICATION_HPP_
#define _AL_APPLICATION_APPLICATION_HPP_

#pragma once

#include <memory>
#include <exception>
#include <stdexcept>

#include "syslog/syslog.hpp"
#include "config/config.hpp"
#include "crypto/crypto.hpp"
#include "audit/base/base-logger.hpp"
#include "audit/file/file-logger.hpp"
#include "interceptor/base-interceptor.hpp"
#include "interceptor/crypto-interceptor.hpp"
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

                const auto interceptor = std::make_shared<al::interceptor::crypto_interceptor>(crypto);
                this->with(interceptor);

                const auto audit_logger = std::make_shared<al::audit::file::file_logger>(interceptor);
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

            virtual const application* with(const std::shared_ptr<al::storage::secure_storage> storage) const {
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

        protected:
            void run_impl(void) const {
                // Initialize storage first, then verify its integrity.
                _storage->initialize();
                const auto storage_state = _storage->verify();
                _logger->info(std::string("Secure storage: ") +
                    (storage_state == al::storage::EMPTY ? "empty" : storage_state == al::storage::GOOD ? "good" :
                    storage_state == al::storage::BAD ? "bad" : storage_state == al::storage::NO_ACCESS ? "no access" : "unknown")
                );

                // Select its current running mode: audit log entry producer or log dumper.
                if (_config->is_dump_mode()) {
                    run_dump_mode_impl();
                }
                else if (_config->is_log_mode()) {
                    run_log_mode_impl();
                }
                else {
                    throw std::runtime_error("Wrong application mode");
                }
            }

            void run_dump_mode_impl(void) const {
            }

            void run_log_mode_impl(void) const {
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            mutable std::shared_ptr<al::config::config> _config;
            mutable std::shared_ptr<al::crypto::crypto> _crypto;
            mutable std::shared_ptr<al::interceptor::base_interceptor> _interceptor;
            mutable std::shared_ptr<al::storage::secure_storage> _storage;
            mutable std::shared_ptr<al::audit::base::base_logger> _audit_logger;
    };
}

#endif /* _AL_APPLICATION_APPLICATION_HPP_ */