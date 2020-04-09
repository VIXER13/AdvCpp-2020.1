cmake_minimum_required(VERSION 3.10)

project(logger)

set(LOGGER_SOURCES "src/LoggerException.cpp"
                   "src/BaseLogger.cpp"
                   "src/StdoutLogger.cpp"
                   "src/StderrLogger.cpp"
                   "src/FileLogger.cpp"
                   "src/Logger.cpp")
set(LOGGER_HEADERS include)
add_library(logger STATIC ${LOGGER_SOURCES})
target_include_directories(logger PUBLIC ${LOGGER_HEADERS})