//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#include <memory>
#include <string>
#include <cstdlib>

#include "tests/base-test.hpp"

class crypto_test: public al::tests::base_test {
    public:
        crypto_test(const std::string suite_description): al::tests::base_test(suite_description) {
        }
};

int main(void) {
    const auto t = std::make_shared<crypto_test>(std::string("CRYPTO TEST"));

    const auto result = t->test("The Crypto tests", [](){
        auto result = true;
        return result;
    });

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
