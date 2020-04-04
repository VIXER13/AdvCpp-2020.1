#include "Logger.hpp"
#include "StdoutLogger.hpp"
#include "StderrLogger.hpp"
#include "FileLogger.hpp"

#include <iostream>

namespace logger {

Logger::Logger() :
    global_logger_(std::make_unique<StdoutLogger>()) {}

Logger& Logger::get_instance() {
    static Logger logger;
    return logger;
}

std::unique_ptr<BaseLogger>& Logger::get_global_logger() noexcept {
    return global_logger_;
}

void Logger::set_global_logger(std::unique_ptr<BaseLogger> other) {
    global_logger_ = std::move(other);
}

void create_stdout_logger(const Level lvl) {
    Logger::get_instance().set_global_logger(std::make_unique<StdoutLogger>(lvl));
}

void create_stderr_logger(const Level lvl) {
    Logger::get_instance().set_global_logger(std::make_unique<StderrLogger>(lvl));
}

void create_file_logger(const std::string& path, const Level lvl) {
    Logger::get_instance().set_global_logger(std::make_unique<FileLogger>(path, lvl));
}

void debug(const std::string& str) {
    Logger::get_instance().get_global_logger()->debug(str);
}

void info(const std::string& str) {
    Logger::get_instance().get_global_logger()->info(str);
}

void warning(const std::string& str) {
    Logger::get_instance().get_global_logger()->warn(str);
}

void error(const std::string& str) {
    Logger::get_instance().get_global_logger()->error(str);
}

void flush() {
    Logger::get_instance().get_global_logger()->flush();
}

}  // namespace logger