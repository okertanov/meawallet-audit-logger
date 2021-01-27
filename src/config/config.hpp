//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CONFIG_CONFIG_HPP_
#define _AL_CONFIG_CONFIG_HPP_

#pragma once

#include <memory>
#include <vector>

#include "config/base-source.hpp"

namespace al::config {
    class config {
        public:
        config(std::vector<std::shared_ptr<al::config::source::base_source>> sources): _sources(sources) {
        }

        virtual ~config() {
        }

        private:
        const std::vector<std::shared_ptr<al::config::source::base_source>> _sources;
    };
}

#endif /* _AL_CONFIG_CONFIG_HPP_ */