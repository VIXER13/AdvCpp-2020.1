#ifndef FILE_LOGGER_HPP
#define FILE_LOGGER_HPP

#include "BaseLogger.hpp"
#include <fstream>

namespace logger {

class FileLogger : public BaseLogger {
 public:
    FileLogger() = default;
    explicit FileLogger(const std::string& path, const Level lvl);
    void flush() override;

 private:
    std::ofstream fout;
    void log(const std::string& str, const Level lvl) override;
};

}  // namespace logger

#endif  // FILE_LOGGER_HPP
