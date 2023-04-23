cmake_minimum_required(VERSION 3.23)
project(red_black_tree)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(red_black_tree main.cpp tree_map.h red_black_tree_test.cpp)

target_link_libraries(
        red_black_tree
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(red_black_tree)
