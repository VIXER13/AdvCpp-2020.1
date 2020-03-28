cmake_minimum_required(VERSION 3.10)

project(Process)

set(SOURCES project/src/main.cpp project/src/simple_dialog.cpp)
set(PROCESS_LIB_SOURCES process_lib/src/process.cpp process_lib/src/descriptor.cpp process_lib/src/pipe.cpp)

set(HEADERS project/include)
set(PROCESS_LIB_HEADERS process_lib/include)

add_library(process STATIC ${PROCESS_LIB_SOURCES})

include_directories(${HEADERS} ${PROCESS_LIB_HEADERS})
add_executable(hw1 ${SOURCES})

target_link_libraries(hw1 process)