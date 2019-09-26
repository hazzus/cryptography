cmake_minimum_required(VERSION 3.5)

project(kasiski)

add_library(encryptor
    encryptor/encryptor.h
    encryptor/encryptor.cpp
    )
include_directories(encryptor)

add_library(decryptor
    decryptor/decryptor.h
    decryptor/decryptor.cpp
    )
include_directories(decryptor)

add_executable(${PROJECT_NAME} "main.cpp")
add_executable(encrypt "encrypt.cpp")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

target_link_libraries(encrypt encryptor)
target_link_libraries(${PROJECT_NAME} decryptor)
