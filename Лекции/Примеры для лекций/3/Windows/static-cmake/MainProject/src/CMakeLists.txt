cmake_minimum_required(VERSION 3.20)

include_directories(${LibEx_SOURCE_DIR}/LibProject/include)
add_executable(MainProject test-client.c)
target_link_libraries(MainProject LibProject)