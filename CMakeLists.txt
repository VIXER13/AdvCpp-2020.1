cmake_minimum_required(VERSION 3.10)

project(Process)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#LIBRARIES

set(FILE_DESCRIPTOR_SOURCES libraries/file_descriptor/src/file_descriptor_exception.cpp libraries/file_descriptor/src/descriptor.cpp libraries/file_descriptor/src/pipe.cpp)
set(FILE_DESCRIPTOR_HEADERS libraries/file_descriptor/include)
add_library(file_descriptor STATIC ${FILE_DESCRIPTOR_SOURCES})
target_include_directories(file_descriptor PUBLIC ${FILE_DESCRIPTOR_HEADERS})

set(PROCESS_SOURCES libraries/process/src/process_exception.cpp libraries/process/src/process.cpp)
set(PROCESS_HEADERS libraries/process/include)
add_library(process STATIC ${PROCESS_SOURCES})
target_include_directories(process PUBLIC ${PROCESS_HEADERS})
target_link_libraries(process file_descriptor)

#EXAMPLES

SET(HW1_EXAMPLE_SOURCES examples/hw1/src/main.cpp examples/hw1/src/simple_dialog.cpp)
SET(HW1_EXAMPLE_HEADERS examples/hw1/include)
add_executable(hw1 ${HW1_EXAMPLE_SOURCES})
target_include_directories(hw1 PUBLIC ${HW1_EXAMPLE_HEADERS})
target_link_libraries(hw1 process)