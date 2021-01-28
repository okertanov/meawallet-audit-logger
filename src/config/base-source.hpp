//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CONFIG_SOURCE_BASE_SOURCE_HPP_
#define _AL_CONFIG_SOURCE_BASE_SOURCE_HPP_

#pragma once

#include <string>

namespace al::config::source {
    class base_source {
        public:
            virtual bool has_key(const std::string& key) const = 0;
            virtual bool has_value(const std::string& key) const = 0;
            virtual std::string get_value(const std::string& key) const = 0;
    };
}

#endif /* _AL_CONFIG_SOURCE_BASE_SOURCE_HPP_ */