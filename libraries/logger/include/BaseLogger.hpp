#ifndef BASE_LOGGER_HPP
#define BASE_LOGGER_HPP

#include <map>
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
    virtual ~BaseLogger() noexcept = default;

 protected:
    const std::string& get_prefix(const Level lvl) const noexcept;

 private:
    Level level_ = Level::INFO;
    inline static const std::map<Level, std::string> message_level_ = {{Level::DEBUG,   "DEBUG: "  },
                                                                       {Level::INFO,    "INFO: "   },
                                                                       {Level::WARNING, "WARNING: "},
                                                                       {Level::ERROR,   "ERROR: "  }};

    virtual void log(const std::string& str, const Level lvl) = 0;
};

class LoggerException : public std::exception {
    std::string message_;

 public:
    explicit LoggerException(const std::string& str) :
        message_(str) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }
};

}  // namespace logger

#endif  // BASE_LOGGER_HPP
