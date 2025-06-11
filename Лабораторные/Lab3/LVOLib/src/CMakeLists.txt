cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_SOURCE_DIR}/LVOLib/include) # каталоги для поиска заголовочных файлов
add_library(LVOLib STATIC LVO.c) # собирает библиотеку из указанных файлов