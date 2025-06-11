cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_SOURCE_DIR}/LVODLibPP/include) # каталоги для поиска заголовочных файлов
add_library(LVODLibPP SHARED LVOCPP.cpp) # собирает библиотеку из указанных файлов
