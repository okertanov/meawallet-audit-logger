//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CRYPTO_CRYPTO_HPP_
#define _AL_CRYPTO_CRYPTO_HPP_

#pragma once

#include <memory>
#include <string>

#include "syslog/syslog.hpp"
#include "crypto/base-crypto.hpp"
#include "crypto/aes/aes-cbc.hpp"

namespace al::crypto {
    class crypto: public al::crypto::base_crypto {
        public:
            crypto() :
                _logger(al::syslog::syslog::create("crypto")),
                _aes_cbc(std::make_unique<al::crypto::aes_cbc>()) {
            }

            virtual ~crypto() {
            }

            virtual const std::string encrypt(const std::string& data, const std::string key, const std::string mac) const {
                const auto encrypted = _aes_cbc->encrypt(data, key, mac);
                //_logger->debug(std::string("DATA IN: '") + data + "'");
                //_logger->debug(std::string("ENCRYPTED: '") + encrypted + "'");
                return encrypted;
            }

            virtual const std::string decrypt(const std::string& data, const std::string key, const std::string mac) const {
                const auto decrypted = _aes_cbc->decrypt(data, key, mac);
                //_logger->debug(std::string("DATA IN: '") + data + "'");
                //_logger->debug(std::string("DECRYPTED: '") + decrypted + "'");
                return decrypted;
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            std::unique_ptr<al::crypto::aes_cbc> _aes_cbc;
    };
}

#endif /* _AL_CRYPTO_CRYPTO_HPP_ */