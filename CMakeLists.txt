cmake_minimum_required(VERSION 3.10)

project(Process)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#hw1
set(PROCESS_LIB_SOURCES process_lib/src/process.cpp process_lib/src/descriptor.cpp process_lib/src/pipe.cpp)
set(PROCESS_LIB_HEADERS process_lib/include)
set(PROCESS_SOURCES hw1/project/src/main.cpp hw1/project/src/simple_dialog.cpp)
set(PROCESS_HEADERS hw1/project/include)

add_library(process STATIC ${PROCESS_LIB_SOURCES})
target_include_directories(process PRIVATE ${PROCESS_LIB_HEADERS})

#include_directories(${PROCESS_LIB_HEADERS})

add_executable(hw1_ ${PROCESS_SOURCES})
target_include_directories(hw1_ PRIVATE ${PROCESS_LIB_HEADERS} ${PROCESS_HEADERS})
target_link_libraries(hw1_ process)

#hw3
set(TCP_TEST_SOURCES tcp_test/server_test.cpp)
set(TCP_LIB_SORCES tcp/src/connection.cpp tcp/src/server.cpp)
set(TCP_LIB_HEADERS tcp/include)

add_library(tcplib STATIC ${TCP_LIB_SORCES})
target_include_directories(tcplib PRIVATE ${TCP_LIB_HEADERS} ${PROCESS_LIB_HEADERS})
target_link_libraries(tcplib process)

add_executable(hw3_ ${TCP_TEST_SOURCES})
target_include_directories(hw3_ PRIVATE ${PROCESS_LIB_HEADERS} ${TCP_LIB_HEADERS} ${PROCESS_LIB_HEADERS})
target_link_libraries(hw3_ tcplib)