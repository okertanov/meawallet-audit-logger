//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_INTERCEPTOR_CRYPTO_INTERCEPTOR_HPP_
#define _AL_INTERCEPTOR_CRYPTO_INTERCEPTOR_HPP_

#pragma once

#include <memory>
#include <string>

#include "interceptor/base-interceptor.hpp"
#include "syslog/syslog.hpp"
#include "crypto/base-crypto.hpp"

namespace al::interceptor {
    class crypto_interceptor: public al::interceptor::base_interceptor {
        public:
            crypto_interceptor(
                std::shared_ptr<al::storage::base_storage> storage,
                std::shared_ptr<al::crypto::base_crypto> crypto
            ) :
                _logger(al::syslog::syslog::create("crypto_interceptor")),
                _storage(storage),
                _crypto(crypto) {
            }

            virtual ~crypto_interceptor() {
            }

            virtual const std::string transform(const std::string& input) const {
                return _crypto->encrypt(input, _storage->get_audit_encryption(), _storage->get_audit_mac());
            }

            virtual const std::string transform_back(const std::string& input) const {
                return _crypto->decrypt(input, _storage->get_audit_encryption(), _storage->get_audit_mac());
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            const std::shared_ptr<al::storage::base_storage> _storage;
            const std::shared_ptr<al::crypto::base_crypto> _crypto;
    };
}

#endif /* _AL_INTERCEPTOR_CRYPTO_INTERCEPTOR_HPP_ */