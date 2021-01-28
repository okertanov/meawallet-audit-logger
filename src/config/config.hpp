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

            bool is_debug() const {
                return false;
            }

            bool is_data_dir_overriden() const {
                return false;
            }

            std::string get_data_dir() const {
                return "";
            }

            bool is_dump_mode() const {
                return false;
            }

            bool is_monitor_mode() const {
                return false;
            }

            bool is_count_mode() const {
                return false;
            }

            bool is_reset_mode() const {
                return false;
            }

            bool is_log_mode() const {
                return false;
            }

            bool is_data_provided() const {
                return false;
            }

            std::string get_data() const {
                return "";
            }

        private:
            const std::vector<std::shared_ptr<al::config::source::base_source>> _sources;
    };
}

#endif /* _AL_CONFIG_CONFIG_HPP_ */