cmake_minimum_required(VERSION 3.10)

project(hw2)

SET(HW2_EXAMPLE_SOURCES "src/main.cpp"
                        "src/LoggerInit.cpp")
SET(HW2_EXAMPLE_HEADERS include)
add_executable(hw2 ${HW2_EXAMPLE_SOURCES})
target_include_directories(hw2 PUBLIC ${HW2_EXAMPLE_HEADERS})
target_link_libraries(hw2 logger)