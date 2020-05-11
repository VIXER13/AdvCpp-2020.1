cmake_minimum_required(VERSION 3.10)

project(http_framework)

set(HTTP_FRAMEWORK_SOURCES "src/HttpServerException.cpp"
                           "src/Connection.cpp"
                           "src/Server.cpp"
                           "src/Coroutine.cpp")
set(HTTP_FRAMEWORK_HEADERS "include")
add_library(http_framework STATIC ${HTTP_FRAMEWORK_SOURCES})
target_include_directories(http_framework PUBLIC ${HTTP_FRAMEWORK_HEADERS})
target_link_libraries(http_framework file_descriptor)
target_link_libraries(http_framework logger)