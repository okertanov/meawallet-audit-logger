//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#include <memory>
#include <string>
#include <cstdlib>

#include "tests/base-test.hpp"

class audit_test: public al::tests::base_test {
    public:
        audit_test(const std::string suite_description): al::tests::base_test(suite_description) {
        }
};

int main(void) {
    const auto t = std::make_shared<audit_test>(std::string("AUDIT TEST"));

    const auto result = t->test("The Audit logging tests", [](){
        auto result = true;
        return result;
    });

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
