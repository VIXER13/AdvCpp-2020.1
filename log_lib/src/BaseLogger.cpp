#include "BaseLogger.hpp"

namespace logger {

BaseLogger::BaseLogger(const Level lvl) :
    level_(lvl) {}

void BaseLogger::debug(const std::string& str) {
    log(str, Level::DEBUG);
}

void BaseLogger::info(const std::string& str) {
    log(str, Level::INFO);
}

void BaseLogger::warn(const std::string& str) {
    log(str, Level::WARNING);
}

void BaseLogger::error(const std::string& str) {
    log(str, Level::ERROR);
}

void BaseLogger::set_level(const Level lvl) noexcept {
    level_ = lvl;
}

Level BaseLogger::level() const noexcept {
    return level_;
}

}  // namespace logger
