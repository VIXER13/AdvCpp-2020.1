#include "StderrLogger.hpp"
#include <iostream>

namespace logger {

StderrLogger::StderrLogger(const Level lvl) :
    BaseLogger(lvl) {}

void StderrLogger::flush() {
    std::cerr << std::flush;
}

void StderrLogger::log(const std::string& str, const Level lvl) {
    if (lvl >= level()) {
        std::cerr << message_level[size_t(lvl)] << str << '\n';
    }
}

}  // namespace logger
