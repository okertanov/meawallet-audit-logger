//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#include <memory>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <cassert>

#include "syslog/syslog.hpp"
#include "syslog/console/console-logger.hpp"
#include "config/config.hpp"
#include "config/base-source.hpp"
#include "config/cmdline-source.hpp"
#include "config/environment-source.hpp"
#include "application/application.hpp"

int main(void) {
    auto ret_code = EXIT_SUCCESS;

    // Configure system logger to use here and inside the application and modules.
    al::syslog::syslog::set_inner_logger(
        std::make_shared<al::syslog::console::console_logger>()
    );
    
    // Create local logger instance for this 'main' scope.
    const auto logger = al::syslog::syslog::create("app-test");

    logger->info("Starting...");

    try {
        const char* argv[] = { "--datadir=storage", "--debug", "--monitor", "--count", "--reset", "--data=\"{...}\"" };
        const auto argc = sizeof(argv) / sizeof(argv[0]);
        const char* envp[] = { "DATADIR=storage", "DEBUG", "MONITOR", "COUNT", "RESET", "DATA=\"{...}\"", nullptr };

        const std::vector<std::shared_ptr<al::config::source::base_source>> config_sources = {
            std::make_shared<al::config::source::environment_source>(envp),
            std::make_shared<al::config::source::cmdline_source>(argc, argv)
        };
        const auto config = std::make_shared<al::config::config>(config_sources);
    }
    catch(std::exception& e) {
        logger->error("Error: ", e);
        ret_code = EXIT_FAILURE;
    }

    logger->info("Done.");

    return ret_code;
}