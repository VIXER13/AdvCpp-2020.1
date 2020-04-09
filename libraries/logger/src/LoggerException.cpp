#include "LoggerException.hpp"

namespace logger {

LoggerException::LoggerException(const std::string& str) :
    message_(str) {}

const char* LoggerException::what() const noexcept {
    return message_.c_str();
}

}  // namespace logger
