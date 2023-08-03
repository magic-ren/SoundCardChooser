
cmake_minimum_required(VERSION 3.18.1)


project("tinyalsa_demo")

file(GLOB_RECURSE src_files *.cpp *.h *.c)

add_library(
        tinyalsa_demo
        SHARED
        ${src_files})


find_library(
        log-lib
        log)


target_link_libraries(
        tinyalsa_demo
        ${log-lib}
        android
)