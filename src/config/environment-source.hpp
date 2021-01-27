//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CONFIG_SOURCE_ENVIRONMENT_SOURCE_HPP_
#define _AL_CONFIG_SOURCE_ENVIRONMENT_SOURCE_HPP_

#pragma once

#include <string>
#include <vector>

#include "config/base-source.hpp"

namespace al::config::source {
    class environment_source: public al::config::source::base_source {
        public:
            environment_source(const char* envp[]) {
                for (; *envp != nullptr; ++envp) {
                    _env.push_back(std::string(*envp));
                }
            }

        private:
            std::vector<std::string> _env;
    };
}

#endif /* _AL_CONFIG_SOURCE_ENVIRONMENT_SOURCE_HPP_ */