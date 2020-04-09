#ifndef LOGGER_EXCEPTION_HPP
#define LOGGER_EXCEPTION_HPP

#include <string>
#include <exception>

namespace logger {

class LoggerException : public std::exception {
    std::string message_;

 public:
    explicit LoggerException(const std::string& str);
    const char* what() const noexcept override;
};

}  // namespace logger

#endif  // LOGGER_EXCEPTION_HPP
