//
// Copyright © 2021 MeaWallet. All rights reserved.
//

#ifndef _AL_INTERCEPTOR_BASE_INTERCEPTOR_HPP_
#define _AL_INTERCEPTOR_BASE_INTERCEPTOR_HPP_

#pragma once

namespace al::interceptor {
    class base_interceptor {
        public:
           base_interceptor() {
           }

           virtual ~base_interceptor() {
           }

           virtual const std::string transform(const std::string& input) const = 0;
           virtual const std::string transform_back(const std::string& input) const = 0;
    };
}

#endif /* _AL_INTERCEPTOR_BASE_INTERCEPTOR_HPP_ */