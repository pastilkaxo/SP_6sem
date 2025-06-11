cmake_minimum_required(VERSION 3.20)

include_directories(${DllEx_SOURCE_DIR}/LibProject/include)
add_library(LibProject SHARED MyLib.c)