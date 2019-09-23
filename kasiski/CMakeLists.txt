cmake_minimum_required(VERSION 2.8)

project(kasiski)

add_library(encryptor encryptor.h encryptor.cpp)

add_executable(${PROJECT_NAME} "main.cpp")
add_executable(encrypt "encrypt.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

target_link_libraries(encrypt encryptor)
