#include <memory>
#include <cstdlib>
#include <exception>

#include "application/application.hpp"
#include "audit/logger.hpp"
#include "syslog/syslog.hpp"
#include "syslog/console/console-logger.hpp"

int main(int argc, char** argv) {
    auto ret_code = EXIT_SUCCESS;

    // 1. Configure system logger
    auto const inner_logger = std::make_shared<al::syslog::console::console_logger>();
    al::syslog::syslog::set_inner_logger(inner_logger);
    
    auto const app_name = std::string(argv[0]);
    auto const logger = al::syslog::syslog::create(app_name);

    logger->info("Starting...");

    try {
        // 2. Retrieve a configuration

        // 3. Botstrap Audit logger amd Crypto

        // 4. Bootstrap an application

    }
    catch(std::exception& e) {
        logger->error("Error:", e);
        ret_code = EXIT_FAILURE;
    }

    logger->info("Done.");

    return ret_code;
}