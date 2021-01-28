//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CONFIG_SOURCE_CMDLINE_SOURCE_HPP_
#define _AL_CONFIG_SOURCE_CMDLINE_SOURCE_HPP_

#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "config/base-source.hpp"

namespace al::config::source {
    class cmdline_source: public al::config::source::base_source {
        public:
            cmdline_source(int argc, const char* argv[])
                : _args(std::vector<std::string> { argv, argv + argc }) {
            }

            virtual ~cmdline_source() {
            }

            virtual bool has_key(const std::string& key) const {
                const auto found = std::find(_args.begin(), _args.end(), key) != _args.end();
                return found;
            }

            virtual bool has_value(const std::string& key) const {
                try {
                    const auto value = get_value(key);
                    const auto has_value = value.length() > 0;
                    return has_value;
                }
                catch(std::runtime_error& e) {
                    // Silently eat exception just report absence
                    return false;
                }
            }

            virtual std::string get_value(const std::string& key) const {
                const auto found_key = std::find(_args.begin(), _args.end(), key);
                if (found_key == _args.end()) {
                    throw std::runtime_error("Key not found");
                }

                const auto found_key_length = found_key->length();

                const auto found_delimiter = found_key->find_first_of("=");
                if (found_delimiter == std::string::npos) {
                    throw std::runtime_error("Value not found");
                }

                if (found_delimiter >= found_key_length) {
                    throw std::runtime_error("Value is empty");
                }

                const auto value = found_key->substr(found_delimiter, found_key_length - 1);
                return value;
            }

        private:
            const std::vector<std::string> _args;
    };
}

#endif /* _AL_CONFIG_SOURCE_CMDLINE_SOURCE_HPP_ */