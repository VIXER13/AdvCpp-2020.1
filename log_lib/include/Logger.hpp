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
    static Logger& get_instance();
    BaseLogger& get_global_logger() noexcept;
    void set_global_logger(std::unique_ptr<BaseLogger> other);
};

std::unique_ptr<BaseLogger> create_stdout_logger(const Level lvl);
std::unique_ptr<BaseLogger> create_stderr_logger(const Level lvl);
std::unique_ptr<BaseLogger> create_file_logger(const std::string& path, const Level lvl);

void debug(const std::string& str);
void info(const std::string& str);
void warning(const std::string& str);
void error(const std::string& str);

void flush();

}  // namespace logger

#endif  // LOGGER_HPP
