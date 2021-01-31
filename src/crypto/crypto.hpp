//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CRYPTO_CRYPTO_HPP_
#define _AL_CRYPTO_CRYPTO_HPP_

#pragma once

#include "crypto/base-crypto.hpp"

namespace al::crypto {
    class crypto: public al::crypto::base_crypto {
        public:
            crypto() {
            }

            virtual ~crypto() {
            }

            virtual const std::string encrypt(const std::string& data, const std::string key, const std::string mac) const {
                return data;
            }

            virtual const std::string decrypt(const std::string& data, const std::string key, const std::string mac) const {
                return data;
            }
    };
}

#endif /* _AL_CRYPTO_CRYPTO_HPP_ */