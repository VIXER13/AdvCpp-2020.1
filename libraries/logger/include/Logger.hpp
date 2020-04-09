#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "BaseLogger.hpp"
#include <memory>

namespace logger {

class Logger {
    std::unique_ptr<BaseLogger> global_logger_;

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

 public:
    static Logger& getInstance();
    BaseLogger& getGlobalLogger() noexcept;
    void setGlobalLogger(std::unique_ptr<BaseLogger> other);
};

std::unique_ptr<BaseLogger> createStdoutLogger(const Level lvl);
std::unique_ptr<BaseLogger> createStderrLogger(const Level lvl);
std::unique_ptr<BaseLogger> createFileLogger(const std::string& path, const Level lvl);

void debug(const std::string& str);
void info(const std::string& str);
void warning(const std::string& str);
void error(const std::string& str);

void flush();

}  // namespace logger

#endif  // LOGGER_HPP
