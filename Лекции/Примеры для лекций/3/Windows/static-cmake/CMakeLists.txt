cmake_minimum_required(VERSION 3.20)

project(LibEx C)
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
add_subdirectory(MainProject)
add_subdirectory(LibProject)