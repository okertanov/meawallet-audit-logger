//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#include <memory>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <vector>

#include "syslog/syslog.hpp"
#include "syslog/console/console-logger.hpp"
#include "config/config.hpp"
#include "config/base-source.hpp"
#include "config/cmdline-source.hpp"
#include "config/environment-source.hpp"
#include "application/application.hpp"

int main(int argc, const char* argv[], const char* envp[]) {
    auto ret_code = EXIT_SUCCESS;

    // Configure system logger to use here and inside the application and modules.
    al::syslog::syslog::set_inner_logger(
        std::make_shared<al::syslog::console::console_logger>()
    );
    
    // Create local logger instance for this 'main' scope.
    const auto logger = al::syslog::syslog::create("audit-logger");

    logger->info("Starting...");

    try {
        // Exctract configuration from the commandline and system environment.
        const std::vector<std::shared_ptr<al::config::source::base_source>> config_sources = {
            std::make_shared<al::config::source::environment_source>(envp),
            std::make_shared<al::config::source::cmdline_source>(argc, argv)
        };
        const auto config = std::make_shared<al::config::config>(config_sources);

        // Create an application instance.
        const auto app = std::make_unique<al::application::application>();

        // Then bootstrap this application with its dependencies and start running it.
        app->with(config)
           ->bootstrap()
           ->run();
    }
    catch(std::exception& e) {
        logger->error("Error: ", e);
        ret_code = EXIT_FAILURE;
    }

    logger->info("Done.");

    return ret_code;
}