#include "syslog/syslog.hpp"

namespace al::syslog
{
    std::shared_ptr<al::syslog::base::base_logger> syslog::_logger;
}
