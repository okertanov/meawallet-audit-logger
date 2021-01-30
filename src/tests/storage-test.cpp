//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#include <memory>
#include <cstdlib>

#include "tests/base-test.hpp"

#include "crypto/base-crypto.hpp"
#include "storage/secure-storage.hpp"

class test_crypto: public al::crypto::base_crypto {
};

class storage_test: public al::tests::base_test {
    public:
        storage_test(const std::string suite_description): al::tests::base_test(suite_description) {
        }
};

int main(void) {
    const auto t = std::make_shared<storage_test>(std::string("STORAGE TEST"));

    const auto result = t->test("The Secure storage tests", [](){
        auto result = true;

        const auto crypto = std::make_shared<test_crypto>();
        const auto storage = std::make_shared<al::storage::secure_storage>(crypto);

        storage->initialize();
        const auto storage_state = storage->verify();

        assert(storage_state == al::storage::EMPTY);

        return result;
    });

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
