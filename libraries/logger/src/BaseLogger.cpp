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

void BaseLogger::setLevel(const Level lvl) noexcept {
    level_ = lvl;
}

Level BaseLogger::level() const noexcept {
    return level_;
}

const std::string& BaseLogger::getPrefix(const Level lvl) const noexcept {
    return message_level_.at(lvl);
}

}  // namespace logger
