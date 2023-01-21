cmake_minimum_required(VERSION 3.16)

project(push-relabel)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set (CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")

# push-relabel
set(${PROJECT_NAME}_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/graph.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
        )
add_executable(push-relabel ${${PROJECT_NAME}_SOURCES})
target_include_directories(push-relabel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_property(TARGET push-relabel PROPERTY CXX_STANDARD 17)

# test-case-generator
set(test_cases
        "${CMAKE_CURRENT_SOURCE_DIR}/src/new-washington.c"
        )
add_executable(graph-generator ${test_cases})
target_include_directories(graph-generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# max-flow control algorithm
set(get-max-flow
        "${CMAKE_CURRENT_SOURCE_DIR}/src/max-flow.cpp"
        )
add_executable(max-flow ${get-max-flow})
target_include_directories(max-flow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

