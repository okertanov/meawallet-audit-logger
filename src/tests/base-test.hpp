//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_TESTS_TEST_BASE_HPP_
#define _AL_TESTS_TEST_BASE_HPP_

#pragma once

#include <memory>
#include <string>
#include <functional>
#include <cassert>

#include "syslog/syslog.hpp"
#include "syslog/console/console-logger.hpp"

namespace al::tests {
    class base_test {
        public:
            base_test(const std::string suite_description): _suite_description(suite_description) {
                al::syslog::syslog::set_inner_logger(
                    std::make_shared<al::syslog::console::console_logger>()
                );
                _logger = al::syslog::syslog::create("TEST");
            }

            virtual ~base_test() {
            }

            virtual void setup() {
            }

            virtual void teardown() {
            }

            virtual void beforeEach() {
            }

            virtual void afterEach() {
            }

            virtual bool test(const std::string test_description, std::function<bool()> test_fn) {
                try {
                    _logger->info("[" + _suite_description + "] " + test_description);
                    const auto result = test_fn();
                    _logger->info(result ? "[OK]" : "[FAILED]");
                    return result;
                }
                catch(std::exception& e) {
                    _logger->error("Error: ", e);
                }

                return false;
            }

        private:
            const std::string _suite_description;
            std::shared_ptr<al::syslog::syslog> _logger;
    };
}

#endif /* _AL_TESTS_TEST_BASE_HPP_ */