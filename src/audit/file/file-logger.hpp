//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_AUDIT_FILE_FILE_LOGGER_HPP_
#define _AL_AUDIT_FILE_FILE_LOGGER_HPP_

#pragma once

#include <memory>

#include "syslog/syslog.hpp"
#include "audit/base/base-logger.hpp"
#include "interceptor/base-interceptor.hpp"

namespace al::audit::file {
    class file_logger: public al::audit::base::base_logger {
        public:
            file_logger(std::shared_ptr<al::interceptor::base_interceptor> interceptor) :
                _logger(al::syslog::syslog::create("file_logger")),
                _interceptor(interceptor) {
            }

            virtual ~file_logger() {
            }

        private:
            const std::shared_ptr<al::syslog::syslog> _logger;
            const std::shared_ptr<al::interceptor::base_interceptor> _interceptor;
    };
}

#endif /* _AL_AUDIT_FILE_FILE_LOGGER_HPP_ */