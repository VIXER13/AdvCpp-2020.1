cmake_minimum_required(VERSION 3.10)

project(hw3)

SET(HW3_EXAMPLE_SOURCES "src/main.cpp"
                        "src/SimpleDialogTcp.cpp")
SET(HW3_EXAMPLE_HEADERS include)
add_executable(hw3 ${HW3_EXAMPLE_SOURCES})
target_include_directories(hw3 PUBLIC ${HW3_EXAMPLE_HEADERS})
target_link_libraries(hw3 tcp)