cmake_minimum_required(VERSION 3.10)

project(epoll_server)

set(EPOLL_SERVER_SOURCES "src/EpollServerException.cpp"
                         "src/Connection.cpp"
                         "src/Server.cpp")
set(EPOLL_SERVER_HEADERS include)
add_library(epoll_server STATIC ${EPOLL_SERVER_SOURCES})
target_include_directories(epoll_server PUBLIC ${EPOLL_SERVER_HEADERS})
target_link_libraries(epoll_server file_descriptor)