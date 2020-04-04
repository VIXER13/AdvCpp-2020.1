#include "FileLogger.hpp"

namespace logger {

FileLogger::FileLogger(const std::string& path, const Level lvl) :
    BaseLogger(lvl), fout_(path) {}

void FileLogger::flush() {
    fout_ << std::flush;
}

void FileLogger::log(const std::string& str, const Level lvl) {
    if (lvl >= level()) {
        fout_ << get_prefix(lvl) << str << '\n';
    }
}

}  // namespace logger
