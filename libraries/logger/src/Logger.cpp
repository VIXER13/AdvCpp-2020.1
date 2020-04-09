#include "Logger.hpp"
#include "StdoutLogger.hpp"
#include "StderrLogger.hpp"
#include "FileLogger.hpp"
#include "LoggerException.hpp"
#include <iostream>

namespace logger {

Logger::Logger() :
    global_logger_(std::make_unique<StdoutLogger>()) {}

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

BaseLogger& Logger::getGlobalLogger() noexcept {
    return *global_logger_.get();
}

void Logger::setGlobalLogger(std::unique_ptr<BaseLogger> other) {
    if (other.get() == nullptr) {
        throw LoggerException{"Logger can't be nullptr"};
    }
    global_logger_ = std::move(other);
}

std::unique_ptr<BaseLogger> createStdoutLogger(const Level lvl) {
    return std::make_unique<StdoutLogger>(lvl);
}

std::unique_ptr<BaseLogger> createStderrLogger(const Level lvl) {
    return std::make_unique<StderrLogger>(lvl);
}

std::unique_ptr<BaseLogger> createFileLogger(const std::string& path, const Level lvl) {
    return std::make_unique<FileLogger>(path, lvl);
}

void debug(const std::string& str) {
    Logger::getInstance().getGlobalLogger().debug(str);
}

void info(const std::string& str) {
    Logger::getInstance().getGlobalLogger().info(str);
}

void warning(const std::string& str) {
    Logger::getInstance().getGlobalLogger().warn(str);
}

void error(const std::string& str) {
    Logger::getInstance().getGlobalLogger().error(str);
}

void flush() {
    Logger::getInstance().getGlobalLogger().flush();
}

}  // namespace logger
