cmake_minimum_required(VERSION 3.5)

project(aes-serpent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native" )

add_library(serpent
    serpent/decoder.h
    serpent/decoder.cpp
    serpent/encoder.h
    serpent/encoder.cpp
    serpent/tables.hpp
    serpent/utils.hpp
    serpent/utils.cpp
    )




add_executable(serpent-encode encode.cpp)
add_executable(serpent-decode decode.cpp)
add_executable(serpent-test
        gtest/gtest_main.cc
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/serpent_testing.cpp)

target_link_libraries(serpent-encode serpent)
target_link_libraries(serpent-decode serpent)
target_link_libraries(serpent-test serpent)