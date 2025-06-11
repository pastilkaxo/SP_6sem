cmake_minimum_required(VERSION 3.20)

include_directories(${SOEx_SOURCE_DIR}/LibProject/include)
add_library(LibProject SHARED mod1.c mod2.c mod3.c)