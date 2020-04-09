#ifndef STDERR_LOGGER_HPP
#define STDERR_LOGGER_HPP

#include "BaseLogger.hpp"

namespace logger {

class StderrLogger : public BaseLogger {
 public:
    StderrLogger() = default;
    explicit StderrLogger(const Level lvl);
    void flush() override;

 private:
    void log(const std::string& str, const Level lvl) override;
};

}  // namespace logger

#endif  // STDERR_LOGGER_HPP
