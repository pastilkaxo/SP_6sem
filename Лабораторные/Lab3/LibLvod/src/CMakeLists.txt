cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_SOURCE_DIR}/LibLvod/include) # каталоги для поиска заголовочных файлов
add_library(LibLvod SHARED libhd.c) # собирает библиотеку из указанных файлов
