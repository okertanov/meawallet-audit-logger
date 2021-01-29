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
    enum secure_storage_state_t {
        EMPTY,
        GOOD,
        BAD,
        NO_ACCESS
    };

    class secure_storage {
        public:
            secure_storage(std::shared_ptr<al::crypto::crypto> crypto) :
                _logger(al::syslog::syslog::create("secure_storage")),
                _crypto(crypto) {
            }

            virtual ~secure_storage() {
            }

            //
            // Gets access to a storage. If no data files - recreates them.
            // Layout is:
            //      storage/
            //          keys/
            //              audit_encryption
            //              audit_mac
            //              storage_encryption
            //              storage_mac
            //          audit_log
            //          secure_storage
            //
            void initialize(void) {
            }

            //
            // Wipes audit_log entries and resets secure_storage to 0.
            //
            void reset(void) {
            }

            //
            // Checks the storage integrity.
            //
            secure_storage_state_t verify(void) {
                return secure_storage_state_t::EMPTY;
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            const std::shared_ptr<al::crypto::crypto> _crypto;
    };
}

#endif /* _AL_STORAGE_SECURE_STORAGE_HPP_ */