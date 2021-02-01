//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CONFIG_CONFIG_HPP_
#define _AL_CONFIG_CONFIG_HPP_

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "config/base-source.hpp"

namespace al::config {
    class config {
        public:
            config(std::vector<std::shared_ptr<al::config::source::base_source>> sources)
                : _sources(sources) {
            }

            virtual ~config() {
            }

            bool is_help() const {
                return _sources[0]->has_key("help") || _sources[1]->has_key("help");
            }

            bool is_debug() const {
                return _sources[0]->has_key("debug") || _sources[1]->has_key("debug");
            }

            bool is_data_dir_overriden() const {
                return (_sources[0]->has_key("datadir") && _sources[0]->has_value("datadir")) ||
                       (_sources[1]->has_key("datadir") && _sources[1]->has_value("datadir"));
            }

            std::string get_data_dir() const {
                if (_sources[0]->has_key("datadir")) {
                    return _sources[0]->get_value("datadir");
                }
                
                if (_sources[1]->has_key("datadir")) {
                    return _sources[1]->get_value("datadir");
                }

                throw std::runtime_error("Value not found");
            }

            bool is_dump_mode() const {
                return !_sources[0]->has_key("entry") && !_sources[1]->has_key("entry");
            }

            bool is_monitor_mode() const {
                return _sources[0]->has_key("monitor") || _sources[1]->has_key("monitor");
            }

            bool is_count_mode() const {
                return _sources[0]->has_key("count") || _sources[1]->has_key("count");
            }

            bool is_reset_mode() const {
                return _sources[0]->has_key("reset") || _sources[1]->has_key("reset");
            }

            bool is_log_mode() const {
                return _sources[0]->has_key("entry") || _sources[1]->has_key("entry");
            }

            bool is_entry_provided() const {
                return (_sources[0]->has_key("entry") && _sources[0]->has_value("entry")) ||
                       (_sources[1]->has_key("entry") && _sources[1]->has_value("entry"));
            }

            std::string get_entry() const {
                if (_sources[0]->has_key("entry")) {
                    return _sources[0]->get_value("entry");
                }
                
                if (_sources[1]->has_key("entry")) {
                    return _sources[1]->get_value("entry");
                }

                throw std::runtime_error("Value not found");
            }

            void show_usage(void) const {
                std::cout
                    << "Usage:" << std::endl
                    << "\taudit-log" << std::endl
                    << "\t\t--help" << std::endl
                    << "\t\t--datadir=storage" << std::endl
                    << "\t\t--debug" << std::endl
                    << "\t\t--monitor" << std::endl
                    << "\t\t--count" << std::endl
                    << "\t\t--reset" << std::endl
                    << "\t\t--entry=\"{...}\"" << std::endl
                    << std::endl;
            }

        private:
            const std::vector<std::shared_ptr<al::config::source::base_source>> _sources;
    };
}

#endif /* _AL_CONFIG_CONFIG_HPP_ */