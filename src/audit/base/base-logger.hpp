//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_AUDIT_BASE_BASE_LOGGER_HPP_
#define _AL_AUDIT_BASE_BASE_LOGGER_HPP_

#pragma once

#include <string>

namespace al::audit::base {
    class base_logger {
        public:
            base_logger() {
            }

            virtual ~base_logger() {
            }

            virtual unsigned long long count_audit_log(void) const = 0;
            virtual const std::string read_audit_log(void) const = 0;
            virtual void append_audit_log(const std::string& line) const = 0;
    };
}

#endif /* _AL_AUDIT_BASE_BASE_LOGGER_HPP_ */