//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_INTERCEPTOR_CRYPTO_INTERCEPTOR_HPP_
#define _AL_INTERCEPTOR_CRYPTO_INTERCEPTOR_HPP_

#pragma once

#include <memory>

#include "interceptor/base-interceptor.hpp"
#include "syslog/syslog.hpp"
#include "crypto/base-crypto.hpp"

namespace al::interceptor {
    class crypto_interceptor: public al::interceptor::base_interceptor {
        public:
            crypto_interceptor(std::shared_ptr<al::crypto::base_crypto> crypto) :
                _logger(al::syslog::syslog::create("crypto_interceptor")),
                _crypto(crypto) {
            }

            virtual ~crypto_interceptor() {
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            const std::shared_ptr<al::crypto::base_crypto> _crypto;
    };
}

#endif /* _AL_INTERCEPTOR_CRYPTO_INTERCEPTOR_HPP_ */