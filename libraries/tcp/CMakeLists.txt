cmake_minimum_required(VERSION 3.10)

project(tcp)

set(TCP_SOURCES "src/TcpException.cpp"
                "src/Connection.cpp"
                "src/Server.cpp")
set(TCP_HEADERS include)
add_library(tcp STATIC ${TCP_SOURCES})
target_include_directories(tcp PUBLIC ${TCP_HEADERS})
target_link_libraries(tcp file_descriptor)