#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "BaseLogger.hpp"

namespace logger {

void create_stdout_logger(const Level lvl);
void create_stderr_logger(const Level lvl);
void create_file_logger(const std::string& path, const Level lvl);

void debug(const std::string& str);
void info(const std::string& str);
void warning(const std::string& str);
void error(const std::string& str);

void flush();

}  // namespace logger

#endif  // LOGGER_HPP
