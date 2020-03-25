#include "FileLogger.hpp"

namespace logger {

FileLogger::FileLogger(const std::string& path, const Level lvl) :
    BaseLogger(lvl), fout(path) {}

void FileLogger::flush() {
    fout << std::flush;
}

void FileLogger::log(const std::string& str, const Level lvl) {
    if (lvl >= level()) {
        fout << message_level[size_t(lvl)] << str << '\n';
    }
}

}  // namespace logger
