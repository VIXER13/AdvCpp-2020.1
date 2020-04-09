cmake_minimum_required(VERSION 3.10)

project(process)

set(PROCESS_SOURCES "src/ProcessException.cpp"
                    "src/Process.cpp")
set(PROCESS_HEADERS include)
add_library(process STATIC ${PROCESS_SOURCES})
target_include_directories(process PUBLIC ${PROCESS_HEADERS})
target_link_libraries(process file_descriptor)