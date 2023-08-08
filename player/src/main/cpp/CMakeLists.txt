
cmake_minimum_required(VERSION 3.18.1)


project("tinyalsa_demo")

#GLOB_RECURSE告诉CMake在指定的目录及其子目录中递归地搜索文件。
#变量src_files代表所有文件
file(GLOB_RECURSE src_files *.cpp *.h *.c)

#将所有文件编译成动态库tinyalsa_demo
add_library(
        tinyalsa_demo
        SHARED
        ${src_files})

#系统上查找log库，并命名为log-lib
find_library(
        log-lib
        log)

#将从系统中查找的log库、ndk提供的android库，链接到自己的编写编译的动态库tinyalsa_demo
target_link_libraries(
        tinyalsa_demo
        ${log-lib}
        android#这里包含在android控制台的打印功能
)