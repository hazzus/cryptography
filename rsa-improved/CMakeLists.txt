cmake_minimum_required(VERSION 3.5)

project(rsa-improved LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(rsa
    rsa/keygen.h
    rsa/keygen.cpp
    rsa/crypt.h
    rsa/crypt.cpp
    rsa/big_integer.h
    rsa/big_integer.cpp)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} rsa gmp)
