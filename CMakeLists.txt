cmake_minimum_required(VERSION 3.10)

project(Process)

set(CMAKE_CXX_STANDARD 17)

#set(SOURCES hw1/project/src/main.cpp hw1/project/src/simple_dialog.cpp)
set(SOURCES tcp_test/main.cpp)
set(PROCESS_LIB_SOURCES process_lib/src/process.cpp process_lib/src/descriptor.cpp process_lib/src/pipe.cpp)
set(TCP_LIB_SORCES tcp/src/connection.cpp tcp/src/server.cpp)

#set(HEADERS hw1/project/include)
set(PROCESS_LIB_HEADERS process_lib/include)
set(TCP_LIB_HEADERS tcp/include)

add_library(process STATIC ${PROCESS_LIB_SOURCES})
add_library(tcplib STATIC ${TCP_LIB_SORCES})

include_directories(${PROCESS_LIB_HEADERS} ${TCP_LIB_HEADERS})
add_executable(proj ${SOURCES})

target_link_libraries(tcplib process)
target_link_libraries(proj tcplib)