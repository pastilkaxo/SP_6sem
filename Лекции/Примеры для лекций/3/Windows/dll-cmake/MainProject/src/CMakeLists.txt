cmake_minimum_required(VERSION 3.20)

include_directories(${DllEx_SOURCE_DIR}/LibProject/include)
add_executable(MainProject WIN32 test.c)
target_link_libraries(MainProject LibProject)