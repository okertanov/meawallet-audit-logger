//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_STORAGE_SECURE_STORAGE_HPP_
#define _AL_STORAGE_SECURE_STORAGE_HPP_

#pragma once

#include <memory>

#include "syslog/syslog.hpp"
#include "crypto/crypto.hpp"

namespace al::storage {
    class secure_storage {
        public:
            secure_storage(std::shared_ptr<al::crypto::crypto> crypto) :
                _logger(al::syslog::syslog::create("secure_storage")),
                _crypto(crypto) {
            }

            virtual ~secure_storage() {
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            const std::shared_ptr<al::crypto::crypto> _crypto;
    };
}

#endif /* _AL_STORAGE_SECURE_STORAGE_HPP_ */