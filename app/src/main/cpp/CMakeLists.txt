
cmake_minimum_required(VERSION 3.18.1)


project("tinyalsa_demo")

file(GLOB src_files *.cpp *.h *.c tinymix.c JNICallbackHelper.h JNICallbackHelper.cpp
        SoundPlayStrategy7L.cpp SoundPlayStrategy8L.cpp SoundPlayStrategy8L7L.cpp SoundPlayStrategyFactory.cpp)

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