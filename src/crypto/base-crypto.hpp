//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CRYPTO_BASE_CRYPTO_HPP_
#define _AL_CRYPTO_BASE_CRYPTO_HPP_

#pragma once

#include <string>

namespace al::crypto {
    class base_crypto {
        public:
            base_crypto() {
            }

            virtual ~base_crypto() {
            }

            virtual const std::string encrypt(const std::string& data, const std::string key, const std::string mac) const = 0;
            virtual const std::string decrypt(const std::string& data, const std::string key, const std::string mac) const = 0;
    };
}

#endif /* _AL_CRYPTO_BASE_CRYPTO_HPP_ */