cmake_minimum_required(VERSION 3.10)

project(hw1)

SET(HW1_EXAMPLE_SOURCES "src/main.cpp"
                        "src/SimpleDialog.cpp")
SET(HW1_EXAMPLE_HEADERS include)
add_executable(hw1 ${HW1_EXAMPLE_SOURCES})
target_include_directories(hw1 PUBLIC ${HW1_EXAMPLE_HEADERS})
target_link_libraries(hw1 process)