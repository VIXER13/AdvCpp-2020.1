cmake_minimum_required(VERSION 3.10)

project(file_descriptor)

set(FILE_DESCRIPTOR_SOURCES "src/FileDescriptorException.cpp"
                            "src/Descriptor.cpp"
                            "src/Pipe.cpp")
set(FILE_DESCRIPTOR_HEADERS include)
add_library(file_descriptor STATIC ${FILE_DESCRIPTOR_SOURCES})
target_include_directories(file_descriptor PUBLIC ${FILE_DESCRIPTOR_HEADERS})