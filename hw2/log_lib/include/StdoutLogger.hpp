#ifndef STDOUT_LOGGER_HPP
#define STDOUT_LOGGER_HPP

#include "BaseLogger.hpp"

namespace logger {

class StdoutLogger : public BaseLogger {
 public:
    StdoutLogger() = default;
    explicit StdoutLogger(const Level lvl);
    void flush() override;

 private:
    void log(const std::string& str, const Level lvl) override;
};

}  // namespace logger

#endif  // STDOUT_LOGGER_HPP
