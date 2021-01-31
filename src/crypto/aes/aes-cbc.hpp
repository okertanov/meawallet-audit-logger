//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CRYPTO_AES_CBC_HPP_
#define _AL_CRYPTO_AES_CBC_HPP_

#pragma once

#include <memory>
#include <string>

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

namespace al::crypto {
    class aes_cbc {
        public:
            aes_cbc() {
            }

            virtual ~aes_cbc() {
            }

            virtual const std::string encrypt(const std::string& data, const std::string key, const std::string mac) const {
                return data;
            }

            virtual const std::string decrypt(const std::string& data, const std::string key, const std::string mac) const {
                return data;
            }
    };
}

#endif /* _AL_CRYPTO_AES_CBC_HPP_ */