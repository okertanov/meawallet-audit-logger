//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#include <memory>
#include <string>
#include <vector>
#include <cstdlib>

#include "tests/base-test.hpp"

#include "config/config.hpp"
#include "config/base-source.hpp"
#include "config/cmdline-source.hpp"
#include "config/environment-source.hpp"

class app_test: public al::tests::base_test {
    public:
        app_test(const std::string suite_description): al::tests::base_test(suite_description) {
        }
};

int main(void) {
    const auto t = std::make_shared<app_test>(std::string("APP TEST"));

    const auto result = t->test("The configurations contain all params", [](){

        const char* argv[] = { "--help", "--datadir=storage", "--debug", "--monitor", "--count", "--reset", "--entry=\"{...}\"" };
        const auto argc = sizeof(argv) / sizeof(argv[0]);
        const char* envp[] = { "HELP", "DATADIR=storage", "DEBUG", "MONITOR", "COUNT", "RESET", "ENTRY=\"{...}\"", nullptr };

        const std::vector<std::shared_ptr<al::config::source::base_source>> config_sources = {
            std::make_shared<al::config::source::environment_source>(envp),
            std::make_shared<al::config::source::cmdline_source>(argc, argv)
        };
        const auto config = std::make_shared<al::config::config>(config_sources);

        auto result = true;

        assert(config->is_help());
        assert(config->is_debug());
        assert(config->is_data_dir_overriden());
        assert(config->get_data_dir() == "storage");
        // This 'is_dump_mode' is FALSE since we have all modes turned on
        assert(false == config->is_dump_mode());
        assert(config->is_monitor_mode());
        assert(config->is_count_mode());
        assert(config->is_reset_mode());
        assert(config->is_log_mode());
        assert(config->is_entry_provided());
        assert(config->get_entry() == "\"{...}\"");
      
        return result;
    });

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}