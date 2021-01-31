//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_STORAGE_BASE_STORAGE_HPP_
#define _AL_STORAGE_BASE_STORAGE_HPP_

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace al::storage {
    enum storage_state_t {
        EMPTY,
        GOOD,
        BAD,
        NO_ACCESS
    };

    struct secure_storage_record_t {
        unsigned long long records_count;
    };

    class base_storage {
        public:
            base_storage() {
            }

            virtual ~base_storage() {
            }

            virtual void initialize(const std::string& base_path = "./") = 0;
            virtual void reset(void) const = 0;
            virtual storage_state_t verify(void) const = 0;

            virtual const std::string get_audit_encryption(void) const = 0;
            virtual const std::string get_audit_mac(void) const = 0;
            virtual const std::string get_storage_encryption(void) const = 0;
            virtual const std::string get_storage_mac(void) const = 0;

            virtual unsigned long long count_audit_log(void) const = 0;
            virtual const std::vector<std::string> read_audit_log(void) const = 0;
            virtual void append_audit_log(const std::string& line) const = 0;

            virtual secure_storage_record_t read_secure_storage_record(void) const = 0;
            virtual void write_secure_storage_record(const secure_storage_record_t record) const = 0;
    };
}

#endif /* _AL_STORAGE_BASE_STORAGE_HPP_ */