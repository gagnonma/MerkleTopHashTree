cmake_minimum_required(VERSION 3.14)
project(merkle_hash_tree C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(merkle_hash_tree
    md5.c
    md5.h
        main.c)
add_executable(test
        md5.c
        md5.h
        newMD5main.c)
