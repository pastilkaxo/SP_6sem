cmake_minimum_required(VERSION 3.20)

include_directories(${CMAKE_SOURCE_DIR}/LVODLib/include)
add_executable(Lab03-im Lab03bi.c)
target_link_libraries(Lab03-im LVODLib)