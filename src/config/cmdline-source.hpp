//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CONFIG_SOURCE_CMDLINE_SOURCE_HPP_
#define _AL_CONFIG_SOURCE_CMDLINE_SOURCE_HPP_

#pragma once

#include <string>
#include <vector>

#include "config/base-source.hpp"

namespace al::config::source {
    class cmdline_source: public al::config::source::base_source {
        public:
            cmdline_source(int argc, const char* argv[])
                : _args(std::vector<std::string> { argv, argv + argc }) {
            }

        private:
            const std::vector<std::string> _args;
    };
}

#endif /* _AL_CONFIG_SOURCE_CMDLINE_SOURCE_HPP_ */