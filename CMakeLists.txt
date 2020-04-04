cmake_minimum_required(VERSION 3.10)

project(Logger)

set(CMAKE_CXX_STANDARD 17)

set(LOGGER_LIB_SOURCES log_lib/src/BaseLogger.cpp log_lib/src/StdoutLogger.cpp log_lib/src/StderrLogger.cpp
                       log_lib/src/FileLogger.cpp log_lib/src/Logger.cpp)
set(LOGGER_LIB_HEADERS log_lib/include)

set(SOURCES logger/src/main.cpp logger/src/logger_init.cpp)
set(HEADERS logger/include)

add_library(logger STATIC ${LOGGER_LIB_SOURCES})

include_directories(${HEADERS} ${LOGGER_LIB_HEADERS})
add_executable(hw2 ${SOURCES})

target_link_libraries(hw2 logger)