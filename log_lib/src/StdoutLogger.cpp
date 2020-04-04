#include "StdoutLogger.hpp"
#include <iostream>

namespace logger {

StdoutLogger::StdoutLogger(const Level lvl) :
    BaseLogger(lvl) {}

void StdoutLogger::flush() {
    std::cout << std::flush;
}

void StdoutLogger::log(const std::string& str, const Level lvl) {
    if (lvl >= level()) {
        std::cout << get_prefix(lvl) << str << '\n';
    }
}

}  // namespace logger
