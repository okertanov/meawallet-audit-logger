//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_CONFIG_SOURCE_ENVIRONMENT_SOURCE_HPP_
#define _AL_CONFIG_SOURCE_ENVIRONMENT_SOURCE_HPP_

#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "config/base-source.hpp"

namespace al::config::source {
    class environment_source: public al::config::source::base_source {
        public:
            environment_source(const char* envp[]) {
                for (; *envp != nullptr; ++envp) {
                    _env.push_back(std::string(*envp));
                }
            }

            virtual ~environment_source() {
            }

            virtual bool has_key(const std::string& key) const {
                auto normalized_key = std::string{ key };
                std::transform(normalized_key.begin(), normalized_key.end(), normalized_key.begin(), ::toupper);

                const auto found = std::find(_env.begin(), _env.end(), normalized_key) != _env.end();
                return found;
            }

            virtual bool has_value(const std::string& key) const {
                try {
                    auto normalized_key = std::string{ key };
                    std::transform(normalized_key.begin(), normalized_key.end(), normalized_key.begin(), ::toupper);

                    const auto value = get_value(normalized_key);
                    const auto has_value = value.length() > 0;
                    return has_value;
                }
                catch(std::runtime_error& e) {
                    // Silently eat exception just report absence
                    return false;
                }
            }

            virtual std::string get_value(const std::string& key) const {
                auto normalized_key = std::string{ key };
                std::transform(normalized_key.begin(), normalized_key.end(), normalized_key.begin(), ::toupper);

                const auto found_key = std::find(_env.begin(), _env.end(), normalized_key);
                if (found_key == _env.end()) {
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
                return value;;
            }

        private:
            std::vector<std::string> _env;
    };
}

#endif /* _AL_CONFIG_SOURCE_ENVIRONMENT_SOURCE_HPP_ */