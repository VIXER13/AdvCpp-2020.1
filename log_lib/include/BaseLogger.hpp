#ifndef BASE_LOGGER_HPP
#define BASE_LOGGER_HPP

#include <array>
#include <string>
#include <exception>

namespace logger {

enum class Level : uint8_t {DEBUG, INFO, WARNING, ERROR};

class BaseLogger {
 public:
    BaseLogger() = default;
    explicit BaseLogger(const Level lvl);
    void debug(const std::string& str);
    void info(const std::string& str);
    void warn(const std::string& str);
    void error(const std::string& str);
    void set_level(const Level lvl) noexcept;
    Level level() const noexcept;

    virtual void flush() = 0;

 protected:
    inline static const std::array<std::string, 4> message_level = {"DEBUG: ", "INFO: ", "WARNING: ", "ERROR: "};

 private:
    Level level_ = Level::INFO;

    virtual void log(const std::string& str, const Level lvl) = 0;
};

class LoggerException : public std::exception {
    std::string message;

 public:
    explicit LoggerException(const std::string& str) :
        message(str) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

}  // namespace logger

#endif  // BASE_LOGGER_HPP
