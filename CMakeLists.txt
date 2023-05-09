cmake_minimum_required(VERSION 3.25)
project(acoustic_ray_tracing)

set(CMAKE_CXX_STANDARD 20)

add_executable(acoustic_ray_tracing main.cpp primitives.cpp)

project(primitive_test)
set(TEST_SRC test.cpp primitives.cpp)

add_executable(primitive_test ${TEST_SRC})
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-g3 -O0")

target_link_libraries(primitive_test gtest gtest_main pthread)
