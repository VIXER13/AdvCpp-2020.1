cmake_minimum_required(VERSION 3.10)

project(Process)

set(SOURCES src/main.cpp src/process.cpp src/simple_dialog.cpp)
set(HEADERS include)

include_directories(${HEADERS})
add_executable(hw1 ${SOURCES})