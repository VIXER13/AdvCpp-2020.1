cmake_minimum_required(VERSION 3.10)

project(Process)

set(SOURCES project/src/main.cpp project/src/simple_dialog.cpp)
set(LIB_SOURCES lib/src/process.cpp)

set(HEADERS project/include)
set(LIB_HEADERS lib/include)

include_directories(${HEADERS} ${LIB_HEADERS})
add_executable(hw1 ${SOURCES} ${LIB_SOURCES})