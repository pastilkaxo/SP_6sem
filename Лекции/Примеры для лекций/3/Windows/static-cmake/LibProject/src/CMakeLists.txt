cmake_minimum_required(VERSION 3.20)

include_directories(${LibEx_SOURCE_DIR}/LibProject/include)
add_library(LibProject STATIC testA.c testW.c)